/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <vector>

#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/ml_static_values.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsHashedNGramsTest : public UnitTestBase {
 protected:
  BatAdsHashedNGramsTest() = default;

  ~BatAdsHashedNGramsTest() override = default;
};

TEST_F(BatAdsHashedNGramsTest, HashingTest) {
  std::string test_string = "tiny";
  const std::unique_ptr<data::Data> text_data =
      std::make_unique<data::TextData>(data::TextData(test_string));

  transformation::HashedNGrams hashed_ngrams;
  const std::unique_ptr<data::Data> hashed_data =
      hashed_ngrams.Apply(text_data);

  ASSERT_EQ(hashed_data->GetType(), data::DataType::VECTOR_DATA);

  data::VectorData* hashed_vect_data =
      static_cast<data::VectorData*>(hashed_data.get());

  // 10000 is the default size
  ASSERT_EQ(hashed_vect_data->GetDimensionCount(), kNumBuckets);

  // Hashes for [t, i, n, y, ti, in, ny, tin, iny, tiny] -- 10 in total
  size_t expected_element_count = 10;
  EXPECT_EQ(hashed_vect_data->GetRawData().size(), expected_element_count);
}

TEST_F(BatAdsHashedNGramsTest, CustomHashingTest) {
  std::string test_string = "tiny";
  const std::unique_ptr<data::Data> text_data =
      std::make_unique<data::TextData>(data::TextData(test_string));

  transformation::HashedNGrams hashed_ngrams(3, std::vector<int>{1, 2, 3});
  const std::unique_ptr<data::Data> hashed_data =
      hashed_ngrams.Apply(text_data);

  ASSERT_EQ(hashed_data->GetType(), data::DataType::VECTOR_DATA);

  data::VectorData* hashed_vect_data =
      static_cast<data::VectorData*>(hashed_data.get());

  ASSERT_EQ(hashed_vect_data->GetDimensionCount(), 3);

  size_t expected_element_count = 3;
  EXPECT_EQ(hashed_vect_data->GetRawData().size(), expected_element_count);
}

}  // namespace ml
}  // namespace ads