/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ml/data/text_data.h"
#include <string>

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsTextDataTest : public UnitTestBase {
 protected:
  BatAdsTextDataTest() = default;

  ~BatAdsTextDataTest() override = default;
};

TEST_F(BatAdsTextDataTest, TextDataInitialization) {
  std::string original_text = "original text";
  data::TextData text_data(original_text);

  EXPECT_EQ(text_data.GetText(), original_text);
}

}  // namespace ml
}  // namespace ads