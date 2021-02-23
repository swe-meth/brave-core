/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string>

#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"

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
  const std::unique_ptr<data::Data> uppercase_data =
      std::make_unique<data::TextData>(uppercase_str);

  transformation::Lowercase lowercase;

  const std::unique_ptr<data::Data> lowercase_data =
      lowercase.Apply(uppercase_data);

  ASSERT_EQ(lowercase_data->GetType(), data::DataType::TEXT_DATA);

  data::TextData* lowercase_text_data =
      static_cast<data::TextData*>(lowercase_data.get());

  EXPECT_FALSE(lowercase_str.compare(lowercase_text_data->GetText()));
}

}  // namespace ml
}  // namespace ads
