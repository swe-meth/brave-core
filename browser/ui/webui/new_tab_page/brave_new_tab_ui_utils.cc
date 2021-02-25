// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/webui/new_tab_page/brave_new_tab_ui_utils.h"

#include "url/gurl.h"

std::string GetValidURLStringForTopSite(const std::string& url) {
  return GURL(url).is_valid() ? url : "https://" + url;
}
