/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/history/core/browser/top_sites_impl.h"
#include "components/ntp_tiles/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(BraveNewTabUITest, ConstantsTest) {
  // Make sure history/ntp_tiles module has proper constants for our NTP
  // requirements.
  constexpr size_t kBraveMaxTopSitesNumber = 12;
  EXPECT_EQ(kBraveMaxTopSitesNumber, history::TopSitesImpl::kTopSitesNumber);
  EXPECT_EQ(kBraveMaxTopSitesNumber, ntp_tiles::kMaxNumCustomLinks);
  EXPECT_EQ(kBraveMaxTopSitesNumber, ntp_tiles::kMaxNumMostVisited);
  EXPECT_EQ(static_cast<int>(kBraveMaxTopSitesNumber), ntp_tiles::kMaxNumTiles);
}
