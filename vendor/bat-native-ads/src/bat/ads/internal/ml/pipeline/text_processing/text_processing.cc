/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/pipeline/pipeline_util.h"
#include "bat/ads/internal/ml/pipeline/text_processing/text_processing.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/transformation/normalization.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {
namespace pipeline {

TextProcessing* TextProcessing::CreateInstance() {
  return new TextProcessing();
}

bool TextProcessing::IsInitialized() {
  return is_initialized_;
}

TextProcessing::TextProcessing() : is_initialized_(false) {
  version_ = 0;
  timestamp_ = "";
  locale_ = "en";
  transformations_ = {};
}

TextProcessing::TextProcessing(const TextProcessing& text_proc) = default;

TextProcessing::~TextProcessing() = default;

TextProcessing::TextProcessing(
    const std::vector<transformation::TransformationPtr>& transformations,
    const model::Linear& linear_model)
    : is_initialized_(true) {
  transformations_ = transformations;
  linear_model_ = linear_model;
}

bool TextProcessing::FromJson(const std::string& json) {
  is_initialized_ = ParsePipelineJSON(json, version_, timestamp_, locale_,
                                      transformations_, linear_model_);

  return is_initialized_;
}

std::map<std::string, double> TextProcessing::Apply(
    const std::shared_ptr<data::Data>& input_data) {
  std::shared_ptr<data::Data> current_data = input_data;
  for (auto& transformation : transformations_) {
    current_data = transformation->Get(current_data);
  }

  if (current_data->GetType() != data::DataType::VECTOR_DATA) {
    return std::map<std::string, double>();
  }

  data::VectorData vector_data =
      *std::static_pointer_cast<data::VectorData>(current_data);

  return linear_model_.TopPredictions(vector_data);
}

const std::map<std::string, double> TextProcessing::GetTopPredictions(
    const std::string& html) {
  data::TextData text_data(html);
  auto predictions = Apply(std::make_shared<data::Data>(text_data));
  double expected_prob = 1.0 / static_cast<double>(predictions.size());
  std::map<std::string, double> rtn;
  for (auto const& prediction : predictions) {
    if (prediction.second > expected_prob) {
      rtn[prediction.first] = prediction.second;
    }
  }
  return rtn;
}

const std::map<std::string, double> TextProcessing::ClassifyPage(
    const std::string& content) {
  if (!IsInitialized()) {
    return std::map<std::string, double>();
  }

  return GetTopPredictions(content);
}

}  // namespace pipeline
}  // namespace ml
}  // namespace ads
