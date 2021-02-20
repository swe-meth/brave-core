/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_
#define BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/values.h"
#include "bat/ads/internal/ml/data/data.h"
#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/ml_aliases.h"
#include "bat/ads/internal/ml/model/linear/linear.h"
#include "bat/ads/internal/ml/pipeline/pipeline_info.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {
namespace pipeline {

class TextProcessing {
 public:
  static TextProcessing* CreateInstance();

  TextProcessing();

  TextProcessing(const TextProcessing& pipeline);

  TextProcessing(const TransformationVector& transformations,
                 const model::Linear& linear_model);

  ~TextProcessing();

  bool IsInitialized();

  void SetInfo(const PipelineInfo& info);

  bool FromJson(const std::string& json);

  std::map<std::string, double> Apply(
      const std::shared_ptr<data::Data>& input_data);

  const std::map<std::string, double> GetTopPredictions(
      const std::string& content);

  const std::map<std::string, double> ClassifyPage(const std::string& content);

 private:
  bool is_initialized_ = false;
  uint16_t version_ = 0;
  std::string timestamp_ = "";
  std::string locale_ = "en";
  TransformationVector transformations_;
  model::Linear linear_model_;
};

}  // namespace pipeline
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_
