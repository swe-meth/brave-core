/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_  // NOLINT

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/values.h"

#include "bat/ads/internal/ml/data/data.h"
#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/model/linear/linear.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {
namespace pipeline {

class TextProcessing {
 public:
  static TextProcessing* CreateInstance();

  TextProcessing();

  TextProcessing(
      const TextProcessing& pipeline);

  TextProcessing(
      const std::vector<transformation::TransformationPtr>& transformations,
      const model::Linear& linear_model);

  ~TextProcessing();

  bool IsInitialized();

  bool FromJson(
      const std::string& json);

  std::map<std::string, double> Apply(
      const std::shared_ptr<data::Data>& input_data);

  const std::map<std::string, double> GetTopPredictions(
      const std::string& content);

  const std::map<std::string, double> ClassifyPage(
      const std::string& content);

 private:
  void GetReverseCategories();

  bool ParseClassifier(
      base::Value* classifier);

  bool ParseTransformations(
      base::Value* transformations);

  bool GetVersionFromJSON(
      base::DictionaryValue* dictionary);

  bool GetTimestampFromJSON(
      base::DictionaryValue* dictionary);

  bool GetLocaleFromJSON(
      base::DictionaryValue* dictionary);

  bool GetTransformationsFromJSON(
      base::DictionaryValue* dictionary);

  bool GetClassifierFromJSON(
      base::DictionaryValue* dictionary);

  bool GetTransformationsFromList(
      base::ListValue List);

  bool is_initialized_;
  uint16_t version_;
  std::string timestamp_;
  std::string locale_;
  std::vector<transformation::TransformationPtr> transformations_;
  model::Linear linear_model_;
};

}  // namespace pipeline
}  // namespace ml_tools
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_H_  // NOLINT
