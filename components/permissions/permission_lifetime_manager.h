/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_H_
#define BRAVE_COMPONENTS_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_H_

#include <map>
#include <memory>
#include <vector>

#include "base/containers/id_map.h"
#include "base/power_monitor/power_observer.h"
#include "base/timer/timer.h"
#include "components/content_settings/core/browser/content_settings_observer.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/permissions/permission_util.h"
#include "content/public/browser/permission_controller_delegate.h"
#include "content/public/browser/permission_type.h"
#include "url/origin.h"

namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class HostContentSettingsMap;

namespace permissions {

class PermissionLifetimeManager : public KeyedService,
                                  public content_settings::Observer,
                                  public base::PowerObserver {
 public:
  PermissionLifetimeManager(content::BrowserContext* browser_context,
                            HostContentSettingsMap* host_content_settings_map);
  PermissionLifetimeManager(const PermissionLifetimeManager&) = delete;
  PermissionLifetimeManager& operator=(const PermissionLifetimeManager&) =
      delete;
  ~PermissionLifetimeManager() override;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // KeyedService:
  void Shutdown() override;

  // content_settings::Observer:
  void OnContentSettingChanged(const ContentSettingsPattern& primary_pattern,
                               const ContentSettingsPattern& secondary_pattern,
                               ContentSettingsType content_type) override;

  // base::PowerObserver:
  void OnResume() override;

  void PermissionDecided(const PermissionRequest* permission_request,
                         const GURL& requesting_origin,
                         const GURL& embedding_origin,
                         ContentSetting content_setting,
                         bool is_one_time);

 private:
  struct ExpiringPermission {
    ContentSettingsType content_settings_type;
    GURL requesting_origin;
    GURL embedding_origin;
    base::Time expiration_time;
  };

  void UpdateExpirationTimer();
  void StopExpirationTimer();

  void OnExpirationTimer();

  content::BrowserContext* const browser_context_ = nullptr;
  HostContentSettingsMap* const host_content_settings_map_ = nullptr;

  base::flat_map<ContentSettingsType,
                 std::map<base::Time, std::vector<ExpiringPermission>>>
      expirations_;

  base::Time current_scheduled_expiration_time_;
  base::OneShotTimer expiration_timer_;
};

}  // namespace permissions

#endif  // BRAVE_COMPONENTS_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_H_
