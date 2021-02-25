/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/permissions/permission_lifetime_manager.h"

#include <algorithm>

#include "base/strings/string_number_conversions.h"
#include "brave/components/permissions/permission_lifetime_pref_names.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/browser/website_settings_info.h"
#include "components/content_settings/core/browser/website_settings_registry.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "services/preferences/public/cpp/scoped_pref_update.h"

namespace permissions {

namespace {

std::string ToStr(base::Optional<base::TimeDelta> lifetime) {
  if (!lifetime)
    return "nullopt";
  return base::NumberToString(lifetime->InSeconds());
}

}  // namespace

PermissionLifetimeManager::PermissionLifetimeManager(
    content::BrowserContext* browser_context,
    HostContentSettingsMap* host_content_settings_map)
    : browser_context_(browser_context),
      host_content_settings_map_(host_content_settings_map) {
  DCHECK(browser_context_);
  DCHECK(host_content_settings_map_);
  host_content_settings_map_->AddObserver(this);
}

PermissionLifetimeManager::~PermissionLifetimeManager() {}

void PermissionLifetimeManager::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterDictionaryPref(prefs::kPermissionLifetimeRoot);
  registry->RegisterDictionaryPref(prefs::kPermissionLifetimeExpirations);
}

void PermissionLifetimeManager::Shutdown() {
  host_content_settings_map_->RemoveObserver(this);
}

void PermissionLifetimeManager::OnContentSettingChanged(
    const ContentSettingsPattern& primary_pattern,
    const ContentSettingsPattern& secondary_pattern,
    ContentSettingsType content_type) {
  auto expirations_it = expirations_.find(content_type);
  if (expirations_it == expirations_.end()) {
    return;
  }

  for (const auto& timed_expirations : expirations_it->second) {
    for (const auto& expiring_permission : timed_expirations.second) {
      if (primary_pattern.IsValid() &&
          !primary_pattern.Matches(expiring_permission.requesting_origin)) {
        continue;
      }
      if (secondary_pattern.IsValid() &&
          !secondary_pattern.Matches(expiring_permission.embedding_origin)) {
        continue;
      }
      if (host_content_settings_map_->GetContentSetting(
              expiring_permission.requesting_origin,
              expiring_permission.embedding_origin,
              content_type) != CONTENT_SETTING_ALLOW) {
        // ERASE RULE.
        LOG(ERROR) << "ERASE ME!";
      }
    }
  }
  LOG(ERROR) << "PermissionLifetimeManager::OnContentSettingChanged "
             << "\nprimary_pattern: " << primary_pattern.ToString()
             << "\nsecondary_pattern: " << secondary_pattern.ToString()
             << "\ntype: "
             << content_settings::WebsiteSettingsRegistry::GetInstance()
                    ->Get(content_type)
                    ->name();
}

void PermissionLifetimeManager::OnResume() {
  StopExpirationTimer();
  UpdateExpirationTimer();
}

void PermissionLifetimeManager::PermissionDecided(
    const PermissionRequest* permission_request,
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    ContentSetting content_setting,
    bool is_one_time) {
  DCHECK(permission_request);
  if (content_setting != ContentSetting::CONTENT_SETTING_ALLOW) {
    return;
  }

  const auto& lifetime = permission_request->GetLifetime();
  if (!lifetime || *lifetime == base::TimeDelta()) {
    return;
  }

  const ContentSettingsType content_settings_type =
      permission_request->GetContentSettingsType();
  const base::Time expiration_time = base::Time::Now() + *lifetime;

  LOG(ERROR) << "PermissionLifetimeManager::PermissionDecided"
             << "\ntype: "
             << content_settings::WebsiteSettingsRegistry::GetInstance()
                    ->Get(content_settings_type)
                    ->name()
             << "\nrequesting_origin: " << requesting_origin
             << "\nembedding_origin: " << embedding_origin
             << "\ncontent_setting: "
             << content_settings::ContentSettingToString(content_setting)
             << "\nlifetime: " << ToStr(permission_request->GetLifetime())
             << " seconds";

  expirations_[content_settings_type][expiration_time].push_back(
      ExpiringPermission{.content_settings_type = content_settings_type,
                         .requesting_origin = requesting_origin,
                         .embedding_origin = embedding_origin,
                         .expiration_time = expiration_time});

  UpdateExpirationTimer();
}

void PermissionLifetimeManager::UpdateExpirationTimer() {
  base::Time nearest_expiration_time(base::Time::Max());
  for (const auto& typed_expirations : expirations_) {
    const auto& timed_expirations_map = typed_expirations.second;
    if (timed_expirations_map.empty()) {
      continue;
    }
    nearest_expiration_time =
        std::min(nearest_expiration_time, timed_expirations_map.begin()->first);
  }

  if (nearest_expiration_time == base::Time::Max()) {
    // Nothing to wait for. Stop the timer and return.
    StopExpirationTimer();
    return;
  }

  if (current_scheduled_expiration_time_ == nearest_expiration_time) {
    // Timer is already correct. Do nothing.
    DCHECK(expiration_timer_.IsRunning());
    return;
  }

  current_scheduled_expiration_time_ = nearest_expiration_time;
  expiration_timer_.Start(
      FROM_HERE, current_scheduled_expiration_time_ - base::Time::Now(),
      base::BindOnce(&PermissionLifetimeManager::OnExpirationTimer,
                     base::Unretained(this)));
}

void PermissionLifetimeManager::StopExpirationTimer() {
  current_scheduled_expiration_time_ = base::Time();
  expiration_timer_.Stop();
}

void PermissionLifetimeManager::OnExpirationTimer() {
  DCHECK(current_scheduled_expiration_time_ != base::Time());

  for (auto& typed_expirations : expirations_) {
    const auto& content_settings_type = typed_expirations.first;
    auto& timed_expirations_map = typed_expirations.second;
    std::vector<base::Time> expiration_times_to_clear;

    for (const auto& timed_expirations : timed_expirations_map) {
      const base::Time& expiration_time = timed_expirations.first;
      if (expiration_time > current_scheduled_expiration_time_) {
        // If we encountered an expiration that is still active, then all next
        // expirations will also be active.
        break;
      }
      expiration_times_to_clear.push_back(expiration_time);
      for (const auto& expiring_permission : timed_expirations.second) {
        host_content_settings_map_->SetContentSettingDefaultScope(
            expiring_permission.requesting_origin,
            expiring_permission.embedding_origin, content_settings_type,
            CONTENT_SETTING_DEFAULT);
      }
    }

    for (const auto& expiration_time_to_clear : expiration_times_to_clear) {
      timed_expirations_map.erase(expiration_time_to_clear);
    }
  }

  UpdateExpirationTimer();
}

}  // namespace permissions