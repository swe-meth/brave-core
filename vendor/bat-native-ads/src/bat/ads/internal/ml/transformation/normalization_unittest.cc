/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string>
#include <vector>

#include "bat/ads/internal/ml/transformation/normalization.h"
#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsNormalizationTest : public UnitTestBase {
 protected:
  BatAdsNormalizationTest() = default;

  ~BatAdsNormalizationTest() override = default;
};

TEST_F(BatAdsNormalizationTest, NormalizationTest) {
  const double kTolerance = 1e-7;

  std::string test_string = "quite a small test string";
  data::TextData text_data(test_string);
  std::shared_ptr<data::Data> data =
      std::make_shared<data::TextData>(text_data);

  transformation::HashedNGrams hashed_ngrams(10, std::vector<int>{3, 4});
  data = hashed_ngrams.Get(data);

  transformation::Normalization normalization;
  data = normalization.Get(data);

  ASSERT_EQ(data->GetType(), data::DataType::VECTOR_DATA);

  const std::shared_ptr<data::VectorData> norm_data =
      std::static_pointer_cast<data::VectorData>(data);

  double s = 0.0;
  for (auto const& x : norm_data->GetRawData()) {
    ASSERT_TRUE(x.second >= 0.0);
    ASSERT_TRUE(x.second <= 1.0);
    s += x.second * x.second;
  }

  EXPECT_TRUE(std::fabs(s - 1.0) < kTolerance);
}

TEST_F(BatAdsNormalizationTest, ChainingTest) {
  std::vector<transformation::TransformationPtr> chain;

  transformation::Lowercase lowercase;
  chain.push_back(std::make_shared<transformation::Lowercase>(lowercase));

  transformation::HashedNGrams hashed_ngrams;
  chain.push_back(
      std::make_shared<transformation::HashedNGrams>(hashed_ngrams));

  transformation::Normalization normalization;
  chain.push_back(
      std::make_shared<transformation::Normalization>(normalization));

  std::string test_string = "TINY";
  data::TextData text_data(test_string);
  std::shared_ptr<data::Data> data =
      std::make_shared<data::TextData>(text_data);
  for (size_t i = 0; i < chain.size(); ++i) {
    data = chain[i]->Get(data);
  }

  ASSERT_EQ(data->GetType(), data::DataType::VECTOR_DATA);

  std::shared_ptr<data::VectorData> vect_data =
      std::static_pointer_cast<data::VectorData>(data);

  EXPECT_EQ(vect_data->GetDimensionCount(), 10000);

  // Hashes for [t, i, n, y, ti, in, ny, tin, iny, tiny] -- 10 in total
  size_t expected_element_count = 10;
  EXPECT_EQ(vect_data->GetRawData().size(), expected_element_count);
}

}  // namespace ml
}  // namespace ads
