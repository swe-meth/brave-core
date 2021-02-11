/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "bat/ads/internal/ml/data/data.h"
#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/model/linear/linear.h"
#include "bat/ads/internal/ml/pipeline/text_processing/text_processing.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ml {

namespace {

const char kHashingModel[] = "hashing_model_min.json";

const char kInvalidModel[] = "invalid_model.json";

const char kTextCMCCrash[] = "text_cmc_crash.txt";

const char kPipelineSpam[] = "pipeline_spam.json";

}  // namespace

class BatAdsTextProcessingPipelineTest : public UnitTestBase {
 protected:
  BatAdsTextProcessingPipelineTest() = default;

  ~BatAdsTextProcessingPipelineTest() override = default;
};

TEST_F(BatAdsTextProcessingPipelineTest, BuildSimplePipeline) {
  const double kTolerance = 1e-6;

  std::vector<transformation::TransformationPtr> transformations;
  transformation::Lowercase lowercase;
  transformations.push_back(
      std::make_shared<transformation::Lowercase>(lowercase));
  transformation::HashedNGrams hashed_ngrams(3, std::vector<int>{1, 2, 3});
  transformations.push_back(
      std::make_shared<transformation::HashedNGrams>(hashed_ngrams));

  std::map<std::string, data::VectorData> weights = {
      {"class_1", data::VectorData(std::vector<double>{1.0, 2.0, 3.0})},
      {"class_2", data::VectorData(std::vector<double>{3.0, 2.0, 1.0})},
      {"class_3", data::VectorData(std::vector<double>{2.0, 2.0, 2.0})}};

  std::map<std::string, double> biases = {
      {"class_1", 0.0}, {"class_2", 0.0}, {"class_3", 0.0}};

  unsigned expected_len = 3;
  model::Linear linear_model(weights, biases);
  pipeline::TextProcessing pipeline =
      pipeline::TextProcessing(transformations, linear_model);

  data::VectorData data_point_3(std::vector<double>{1.0, 0.0, 0.0});

  auto data_point_3_res = linear_model.Predict(data_point_3);
  ASSERT_EQ(data_point_3_res.size(), expected_len);

  std::string test_string = "Test String";
  auto res = pipeline.GetTopPredictions(test_string);
  ASSERT_TRUE(res.size() && res.size() <= expected_len);
  for (auto const& pred : res) {
    EXPECT_TRUE(pred.second > -kTolerance && pred.second < 1.0 + kTolerance);
  }
}

TEST_F(BatAdsTextProcessingPipelineTest, TestLoadFromJson) {
  const base::Optional<std::string> opt_value =
      ReadFileFromTestPathToString(kPipelineSpam);
  ASSERT_TRUE(opt_value.has_value());

  const std::string json = opt_value.value();
  pipeline::TextProcessing pipeline;
  bool load_success = pipeline.FromJson(json);
  ASSERT_TRUE(load_success);

  std::vector<std::string> train_texts = {
      "This is a spam email.", "Another spam trying to sell you viagra",
      "Message from mom with no real subject",
      "Another messase from mom with no real subject", "Yadayada"};
  std::vector<std::string> train_labels = {"spam", "spam", "ham", "ham",
                                           "junk"};

  for (size_t i = 0; i < train_texts.size(); i++) {
    std::shared_ptr<data::Data> text_data = 
        std::make_shared<data::TextData>(data::TextData(train_texts[i]));
    auto preds = pipeline.Apply(text_data);
    for (auto const& pred : preds) {
      double other_prediction = pred.second;
      EXPECT_TRUE(preds[train_labels[i]] >= other_prediction);
    }
  }
}

TEST_F(BatAdsTextProcessingPipelineTest, InitValidModelTest) {
  pipeline::TextProcessing text_proc_pipeline;

  const base::Optional<std::string> opt_value =
      ReadFileFromTestPathToString(kHashingModel);
  ASSERT_TRUE(opt_value.has_value());

  const std::string model_json = opt_value.value();
  EXPECT_TRUE(text_proc_pipeline.FromJson(model_json));
}

TEST_F(BatAdsTextProcessingPipelineTest, InvalidModelTest) {
  pipeline::TextProcessing text_proc_pipeline;

  const base::Optional<std::string> opt_value =
      ReadFileFromTestPathToString(kInvalidModel);
  ASSERT_TRUE(opt_value.has_value());

  const std::string model_json = opt_value.value();
  EXPECT_FALSE(text_proc_pipeline.FromJson(model_json));
}

TEST_F(BatAdsTextProcessingPipelineTest, EmptyModelTest) {
  pipeline::TextProcessing text_proc_pipeline;
  const std::string empty_model_json = "{}";
  EXPECT_FALSE(text_proc_pipeline.FromJson(empty_model_json));
}

TEST_F(BatAdsTextProcessingPipelineTest, MissingModelTest) {
  pipeline::TextProcessing text_proc_pipeline;
  const std::string missing_model_json = "";
  EXPECT_FALSE(text_proc_pipeline.FromJson(missing_model_json));
}

TEST_F(BatAdsTextProcessingPipelineTest, TopPredUnitTest) {
  pipeline::TextProcessing text_proc_pipeline;

  const base::Optional<std::string> opt_value =
      ReadFileFromTestPathToString(kHashingModel);
  ASSERT_TRUE(opt_value.has_value());

  const std::string model_json = opt_value.value();
  EXPECT_TRUE(text_proc_pipeline.FromJson(model_json));

  std::string test_page = "ethereum bitcoin bat zcash crypto tokens!";
  auto preds = text_proc_pipeline.ClassifyPage(test_page);
  EXPECT_TRUE(preds.size());
  EXPECT_TRUE(preds.size() < 100);
  EXPECT_TRUE(preds.count("crypto-crypto"));
  for (auto const& pred: preds) {
    EXPECT_TRUE(pred.second <= preds["crypto-crypto"]);
  }
}

TEST_F(BatAdsTextProcessingPipelineTest, TextCMCCrashTest) {
  pipeline::TextProcessing text_proc_pipeline;

  const base::Optional<std::string> opt_value =
      ReadFileFromTestPathToString(kHashingModel);
  ASSERT_TRUE(opt_value.has_value());

  const std::string model_json = opt_value.value();
  EXPECT_TRUE(text_proc_pipeline.FromJson(model_json));

  const base::Optional<std::string> opt_text_value =
      ReadFileFromTestPathToString(kTextCMCCrash);
  ASSERT_TRUE(opt_text_value.has_value());
  const std::string bad_text = opt_text_value.value();

  auto preds = text_proc_pipeline.ClassifyPage(bad_text);
  EXPECT_TRUE(preds.size() < 100);
  EXPECT_TRUE(preds.size() > 2);
  EXPECT_TRUE(preds.count("personal finance-personal finance"));
  for (auto const& pred: preds) {
    EXPECT_TRUE(pred.second <= preds["personal finance-personal finance"]);
  }
}

}  // namespace ml
}  // namespace ads
