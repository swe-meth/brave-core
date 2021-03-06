// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_EXTENSIONS_API_BRAVE_TODAY_API_H_
#define BRAVE_BROWSER_EXTENSIONS_API_BRAVE_TODAY_API_H_

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class BraveTodayGetHostnameFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("braveToday.getHostname", UNKNOWN)

 protected:
  ~BraveTodayGetHostnameFunction() override {}

  ResponseAction Run() override;
};

class BraveTodayGetRegionUrlPartFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("braveToday.getRegionUrlPart", UNKNOWN)

 protected:
  ~BraveTodayGetRegionUrlPartFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // BRAVE_BROWSER_EXTENSIONS_API_BRAVE_TODAY_API_H_
