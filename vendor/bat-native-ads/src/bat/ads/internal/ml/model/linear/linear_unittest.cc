/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>
#include <vector>

#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/model/linear/linear.h"

#include "bat/ads/internal/json_helper.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

class BatAdsLinearSVMTest : public UnitTestBase {
 protected:
  BatAdsLinearSVMTest() = default;

  ~BatAdsLinearSVMTest() override = default;
};

TEST_F(BatAdsLinearSVMTest, ThreeClassesPredictionTest) {
  std::map<std::string, data::VectorData> weights = {
      {"class_1", data::VectorData(std::vector<double>{1.0, 0.0, 0.0})},
      {"class_2", data::VectorData(std::vector<double>{0.0, 1.0, 0.0})},
      {"class_3", data::VectorData(std::vector<double>{0.0, 0.0, 1.0})}};

  std::map<std::string, double> biases = {
      {"class_1", 0.0}, {"class_2", 0.0}, {"class_3", 0.0}};

  model::Linear linear(weights, biases);

  data::VectorData class1_data_vector(std::vector<double>{1.0, 0.0, 0.0});
  auto res1 = linear.Predict(class1_data_vector);
  ASSERT_TRUE(res1["class_1"] > res1["class_2"]);
  ASSERT_TRUE(res1["class_1"] > res1["class_3"]);

  data::VectorData class2_data_vector(std::vector<double>{0.0, 1.0, 0.0});
  auto res2 = linear.Predict(class2_data_vector);
  ASSERT_TRUE(res2["class_2"] > res2["class_1"]);
  ASSERT_TRUE(res2["class_2"] > res2["class_3"]);

  auto class3_data_vector =
      data::VectorData(std::vector<double>{0.0, 1.0, 2.0});
  auto res3 = linear.Predict(class3_data_vector);
  EXPECT_TRUE(res3["class_3"] > res3["class_1"] &&
              res3["class_3"] > res3["class_2"]);
}

TEST_F(BatAdsLinearSVMTest, BiasesPredictionTest) {
  std::map<std::string, data::VectorData> weights = {
      {"class_1", data::VectorData(std::vector<double>{1.0, 0.0, 0.0})},
      {"class_2", data::VectorData(std::vector<double>{0.0, 1.0, 0.0})},
      {"class_3", data::VectorData(std::vector<double>{0.0, 0.0, 1.0})}};

  std::map<std::string, double> biases = {
      {"class_1", 0.5}, {"class_2", 0.25}, {"class_3", 1.0}};

  model::Linear linear_biased(weights, biases);

  data::VectorData avg_vector(std::vector<double>{1.0, 1.0, 1.0});
  auto res = linear_biased.Predict(avg_vector);
  EXPECT_TRUE(res["class_3"] > res["class_1"] &&
              res["class_3"] > res["class_2"] &&
              res["class_1"] > res["class_2"]);
}

TEST_F(BatAdsLinearSVMTest, BinaryClassifierPredictionTest) {
  std::map<std::string, data::VectorData> weights = {
      {"the_only_class", data::VectorData(std::vector<double>{0.3, 0.2, 0.25})},
  };

  std::map<std::string, double> biases = {
      {"the_only_class", -0.45},
  };

  model::Linear linear(weights, biases);

  data::VectorData data_vector_0(std::vector<double>{1.07, 1.52, 0.91});
  auto res_0 = linear.Predict(data_vector_0);
  ASSERT_EQ(res_0.size(), static_cast<size_t>(1));

  data::VectorData data_vector_1(std::vector<double>{1.11, 1.63, 1.21});
  auto res_1 = linear.Predict(data_vector_1);
  ASSERT_EQ(res_1.size(), static_cast<size_t>(1));

  EXPECT_TRUE(res_0["the_only_class"] < 0.5 && res_1["the_only_class"] > 0.5);
}

TEST_F(BatAdsLinearSVMTest, TopPredictionsTest) {
  std::map<std::string, data::VectorData> weights = {
      {"class_1", data::VectorData(std::vector<double>{1.0, 0.5, 0.8})},
      {"class_2", data::VectorData(std::vector<double>{0.3, 1.0, 0.7})},
      {"class_3", data::VectorData(std::vector<double>{0.6, 0.9, 1.0})},
      {"class_4", data::VectorData(std::vector<double>{0.7, 1.0, 0.8})},
      {"class_5", data::VectorData(std::vector<double>{1.0, 0.2, 1.0})}};

  std::map<std::string, double> biases = {{"class_1", 0.21},
                                          {"class_2", 0.22},
                                          {"class_3", 0.23},
                                          {"class_4", 0.22},
                                          {"class_5", 0.21}};

  model::Linear linear_biased(weights, biases);

  data::VectorData point_1(std::vector<double>{1.0, 0.99, 0.98, 0.97, 0.96});
  auto res_1 = linear_biased.TopPredictions(point_1);
  ASSERT_EQ(res_1.size(), static_cast<size_t>(5));

  data::VectorData point_2(std::vector<double>{0.83, 0.79, 0.91, 0.87, 0.82});
  auto res_2 = linear_biased.TopPredictions(point_2, 2);
  ASSERT_EQ(res_2.size(), static_cast<size_t>(2));

  data::VectorData point_3(std::vector<double>{0.92, 0.95, 0.85, 0.91, 0.73});
  auto res_3 = linear_biased.TopPredictions(point_3, 1);
  EXPECT_EQ(res_3.size(), static_cast<size_t>(1));
}

}  // namespace ml
}  // namespace ads
