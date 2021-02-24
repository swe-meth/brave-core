/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>
#include <map>
#include <memory>
#include <string>

#include "bat/ads/internal/ml/ml_util.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsMLToolsUtilTest : public UnitTestBase {
 protected:
  BatAdsMLToolsUtilTest() = default;

  ~BatAdsMLToolsUtilTest() override = default;
};

TEST_F(BatAdsMLToolsUtilTest, SoftmaxTest) {
  // Arrange
  const double kTolerance = 1e-8;

  std::map<std::string, double> group_1 = {
      {"c1", -1.0}, {"c2", 2.0}, {"c3", 3.0}};

  // Act
  PredictionMap sm = Softmax(group_1);

  double sum = 0.0;
  for (auto const& x : sm) {
    sum += x.second;
  }

  // Assert
  ASSERT_TRUE(sm["c3"] > sm["c1"]);
  ASSERT_TRUE(sm["c3"] > sm["c2"]);
  ASSERT_TRUE(sm["c2"] > sm["c1"]);
  ASSERT_TRUE(sm["c1"] > 0.0);
  ASSERT_TRUE(sm["c3"] < 1.0);
  EXPECT_TRUE(sum - 1.0 < kTolerance);
}

TEST_F(BatAdsMLToolsUtilTest, ExtendedSoftmaxTest) {
  // Arrange
  const double kTolerance = 1e-8;

  std::map<std::string, double> group_1 = {
      {"c1", 0.0}, {"c2", 1.0}, {"c3", 2.0}};

  std::map<std::string, double> group_2 = {
      {"c1", 3.0}, {"c2", 4.0}, {"c3", 5.0}};

  // Act
  PredictionMap sm_1 = Softmax(group_1);
  PredictionMap sm_2 = Softmax(group_2);

  // Assert
  ASSERT_TRUE(std::fabs(sm_1["c1"] - sm_2["c1"]) < kTolerance);
  ASSERT_TRUE(std::fabs(sm_1["c2"] - sm_2["c2"]) < kTolerance);
  ASSERT_TRUE(std::fabs(sm_1["c3"] - sm_2["c3"]) < kTolerance);

  EXPECT_TRUE(std::fabs(sm_1["c1"] - 0.09003057) < kTolerance &&
              std::fabs(sm_1["c2"] - 0.24472847) < kTolerance &&
              std::fabs(sm_1["c3"] - 0.66524095) < kTolerance);
}

TEST_F(BatAdsMLToolsUtilTest, TransformationCopyTest) {
  // Arrange
  transformation::Normalization normalization;
  TransformationPtr t_ptr =
      std::make_unique<transformation::Normalization>(normalization);

  // Act
  TransformationPtr t_ptr_copy = GetTransformationCopy(t_ptr);

  // Assert
  EXPECT_EQ(t_ptr_copy->GetType(),
            transformation::TransformationType::NORMALIZATION);
}

TEST_F(BatAdsMLToolsUtilTest, TransformationVectorCopyTest) {
  // Arrange
  const size_t kVectorSize = 2;

  TransformationVector tr_vect;
  transformation::HashedNGrams hashed_ngrams;
  tr_vect.push_back(
      std::make_unique<transformation::HashedNGrams>(hashed_ngrams));

  transformation::Normalization normalization;
  tr_vect.push_back(
      std::make_unique<transformation::Normalization>(normalization));

  // Act
  TransformationVector tr_vect_copy = GetTransformationVectorCopy(tr_vect);

  // Assert
  ASSERT_EQ(tr_vect_copy.size(), kVectorSize);
  EXPECT_TRUE(tr_vect_copy[0]->GetType() ==
                  transformation::TransformationType::HASHED_NGRAMS &&
              tr_vect_copy[1]->GetType() ==
                  transformation::TransformationType::NORMALIZATION);
}

}  // namespace ml
}  // namespace ads
