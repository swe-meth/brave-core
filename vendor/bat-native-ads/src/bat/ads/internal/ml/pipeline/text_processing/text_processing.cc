/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <memory>

#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/ml_aliases.h"
#include "bat/ads/internal/ml/ml_util.h"
#include "bat/ads/internal/ml/pipeline/pipeline_info.h"
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

TextProcessing::TextProcessing() : is_initialized_(false) {}

TextProcessing::TextProcessing(const TextProcessing& text_proc) {
  is_initialized_ = text_proc.is_initialized_;
  version_ = text_proc.version_;
  timestamp_ = text_proc.timestamp_;
  locale_ = text_proc.locale_;
  linear_model_ = text_proc.linear_model_;
  transformations_ = GetTransformationVectorCopy(text_proc.transformations_);
}

TextProcessing::~TextProcessing() = default;

TextProcessing::TextProcessing(const TransformationVector& transformations,
                               const model::Linear& linear_model)
    : is_initialized_(true) {
  linear_model_ = linear_model;
  transformations_ = GetTransformationVectorCopy(transformations);
}

void TextProcessing::SetInfo(const PipelineInfo& info) {
  version_ = info.version;
  timestamp_ = info.timestamp;
  locale_ = info.locale;
  linear_model_ = info.linear_model;
  transformations_ = GetTransformationVectorCopy(info.transformations);
}

bool TextProcessing::FromJson(const std::string& json) {
  base::Optional<PipelineInfo> pipeline_info = ParsePipelineJSON(json);

  if (pipeline_info.has_value()) {
    SetInfo(pipeline_info.value());
    is_initialized_ = true;
  } else {
    is_initialized_ = false;
  }

  return is_initialized_;
}

std::map<std::string, double> TextProcessing::Apply(
    const std::unique_ptr<data::Data>& input_data) {
  data::VectorData vector_data;
  size_t transformation_count = transformations_.size();

  if (!transformation_count) {
    DCHECK(input_data->GetType() == data::DataType::VECTOR_DATA);
    vector_data = *static_cast<data::VectorData*>(input_data.get());
  } else {
    std::unique_ptr<data::Data> current_data =
        transformations_[0]->Apply(input_data);
    for (size_t i = 1; i < transformation_count; ++i) {
      current_data = transformations_[i]->Apply(current_data);
    }

    DCHECK(current_data->GetType() == data::DataType::VECTOR_DATA);
    vector_data = *static_cast<data::VectorData*>(current_data.get());
  }

  return linear_model_.TopPredictions(vector_data);
}

const std::map<std::string, double> TextProcessing::GetTopPredictions(
    const std::string& html) {
  data::TextData text_data(html);
  auto predictions = Apply(std::make_unique<data::TextData>(text_data));
  double expected_prob =
      1.0 / std::max(1.0, static_cast<double>(predictions.size()));
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
