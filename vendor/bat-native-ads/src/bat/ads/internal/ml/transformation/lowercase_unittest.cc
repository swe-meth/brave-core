/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/data/text_data.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsLowercaseTest : public UnitTestBase {
 protected:
  BatAdsLowercaseTest() = default;

  ~BatAdsLowercaseTest() override = default;
};

TEST_F(BatAdsLowercaseTest, LowercaseTest) {
  const std::string uppercase_str = "LOWER CASE";
  const std::string lowercase_str = "lower case";
  const std::shared_ptr<data::Data> uppercase_data =
      std::make_shared<data::TextData>(uppercase_str);

  transformation::Lowercase lowercase;

  const std::shared_ptr<data::Data> lowercase_data =
      lowercase.Get(uppercase_data);

  ASSERT_EQ(lowercase_data->GetType(), data::DataType::TEXT_DATA);

  const std::shared_ptr<data::TextData> lowercase_text_data =
      std::static_pointer_cast<data::TextData>(lowercase_data);

  EXPECT_FALSE(lowercase_str.compare(lowercase_text_data->GetText()));
}

}  // namespace ml
}  // namespace ads
