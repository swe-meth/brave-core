/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include "brave/net/unstoppable_domains/constants.h"

namespace {

// Hide Unstoppable Domains resolver in the custom provider list in settings
// because it will be used for name resolution only for TLDs from Unstoppable
// Domains, instead of a global DoH setting.
bool ShouldSkipDoHServer(const std::string& server) {
  return server == unstoppable_domains::kDoHResolver;
}

}  // namespace

#define BRAVE_CREATE_SECURE_DNS_SETTING_DICT           \
  if (ShouldSkipDoHServer(doh_server.server_template)) \
    continue;

#include "../../../../../../../chrome/browser/ui/webui/settings/settings_secure_dns_handler.cc"
#undef BRAVE_CREATE_SECURE_DNS_SETTING_DICT
