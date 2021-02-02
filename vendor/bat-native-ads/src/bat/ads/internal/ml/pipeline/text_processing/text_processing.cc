/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "bat/ads/internal/ml/pipeline/text_processing/text_processing.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/transformation/transformation.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/normalization.h"

#include "base/json/json_reader.h"
#include "base/values.h"

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

TextProcessing::TextProcessing (
    const TextProcessing& text_proc) = default;

TextProcessing::~TextProcessing() = default;

TextProcessing::TextProcessing(
    const std::vector<transformation::TransformationPtr>& transformations,
    const model::Linear& linear_model) : is_initialized_(true) {
  transformations_ = transformations;
  linear_model_ = linear_model;
}

bool TextProcessing::FromJson(
    const std::string& json) {
  base::Optional<base::Value> root = base::JSONReader::Read(json);
  if (!root) {
    return false;
  }
  base::Value* version =
      root->FindKeyOfType("version", base::Value::Type::INTEGER);
  if (!version) {
    return false;
  }
  int version_number;
  bool parsed_version_number = version->GetAsInteger(&version_number);
  if (!parsed_version_number) {
    return false;
  }
  version_ = version_number;

  base::Value* timestamp = root->FindKey("timestamp");
  if (!timestamp) {
    return false;
  }
  std::string parsed_timestamp;
  bool parsed_timestamp_success = timestamp->GetAsString(&parsed_timestamp);
  if (!parsed_timestamp_success) {
    return false;
  }
  timestamp_ = parsed_timestamp;

  base::Value* locale = root->FindKey("locale");
  if (!locale) {
    return false;
  }
  std::string parsed_locale;
  bool parsed_locale_success = locale->GetAsString(&parsed_locale);
  if (!parsed_locale_success) {
    return false;
  }
  locale_ = parsed_locale;

  base::Value* transformations = root->FindKey("transformations");

  bool loaded_transformations = ParseTransformations(transformations);
  if (!loaded_transformations) {
    return false;
  }
  base::Value* classifier = root->FindKey("classifier");
  if (!ParseClassifier(classifier)) {
    return false;
  }

  is_initialized_ = true;
  return true;
}

bool TextProcessing::ParseTransformations(
    base::Value* transformations) {
  if (!transformations->is_list()) {
    return false;
  }
  std::vector<transformation::TransformationPtr> transformation_sequence;
  for (size_t i = 0; i < transformations->GetList().size(); i++) {
    const base::Value& transformation = transformations->GetList()[i];
    const base::Value* transformation_type =
        transformation.FindKey("transformation_type");
    if (!transformation_type) {
      return false;
    }
    std::string parsed_transformation_type;
    bool parsed_transformation_success =
        transformation_type->GetAsString(&parsed_transformation_type);
    if (!parsed_transformation_success) {
      return false;
    }
    if (parsed_transformation_type.compare("TO_LOWER") == 0) {
      transformation_sequence.push_back(
          std::make_shared<transformation::Lowercase>(transformation::Lowercase()));
    }
    if (parsed_transformation_type.compare("NORMALIZE") == 0) {
      transformation_sequence.push_back(
          std::make_shared<transformation::Normalization>(transformation::Normalization()));
    }
    if (parsed_transformation_type.compare("HASHED_NGRAMS") == 0) {
      const base::Value* transformation_params =
          transformation.FindKey("params");
      if (!transformation_params) {
        return false;
      }
      const base::Value* nb = transformation_params->FindKey("num_buckets");
      int num_buckets;
      bool parsed_num_buckets = nb->GetAsInteger(&num_buckets);
      if (!parsed_num_buckets) {
        return false;
      }
      const base::Value* n_gram_sizes =
          transformation_params->FindKey("ngrams_range");
      if (!n_gram_sizes->is_list()) {
        return false;
      }
      std::vector<int> ngram_range = {};
      for (size_t i = 0; i < n_gram_sizes->GetList().size(); i++) {
        const base::Value& n = n_gram_sizes->GetList()[i];
        ngram_range.push_back(n.GetInt());
      }
      transformation::HashedNGrams hashed_ngrams(num_buckets, ngram_range);
      transformation_sequence.push_back(
          std::make_shared<transformation::HashedNGrams>(hashed_ngrams));
    }
  }
  transformations_ = transformation_sequence;
  return true;
}

bool TextProcessing::ParseClassifier(
    base::Value* classifier) {
  std::vector<std::string> classes;
  base::Value* classifier_type = classifier->FindKey("classifier_type");

  if (!classifier_type) {
    return false;
  }

  std::string parsed_classifier_type;
  bool parsed_classifier_type_success =
      classifier_type->GetAsString(&parsed_classifier_type);
  if (!parsed_classifier_type_success) {
    return false;
  }

  if (parsed_classifier_type.compare("LINEAR")) {
    return false;
  }

  base::Value* specified_classes = classifier->FindKey("classes");
  if (!specified_classes->is_list()) {
    return false;
  }

  for (size_t i = 0; i < specified_classes->GetList().size(); i++) {
    const base::Value& class_name = specified_classes->GetList()[i];
    classes.push_back(class_name.GetString());
  }

  base::Value* class_weights = classifier->FindKey("class_weights");
  if (!class_weights->is_dict()) {
    return false;
  }

  std::map<std::string, data::VectorData> weights;
  for (size_t i = 0; i < classes.size(); i++) {
    base::Value* this_class = class_weights->FindKey(classes[i]);
    if (!this_class->is_list()) {
      return false;
    }
    std::vector<double> class_coef_weights;
    for (size_t j = 0; j < this_class->GetList().size(); j++) {
      const base::Value& weight = this_class->GetList()[j];
      class_coef_weights.push_back(static_cast<double>(weight.GetDouble()));
    }
    weights[classes[i]] = data::VectorData(class_coef_weights);
  }

  std::map<std::string, double> specified_biases;
  base::Value* biases = classifier->FindKey("biases");
  if (!biases->is_list()) {
    return false;
  }
  if (biases->GetList().size() != classes.size()) {
    return false;
  }
  for (size_t i = 0; i < biases->GetList().size(); i++) {
    const base::Value& this_bias = biases->GetList()[i];
    specified_biases.insert(
        {classes.at(i), static_cast<double>(this_bias.GetDouble())});
  }
  linear_model_ = model::Linear(weights, specified_biases);
  return true;
}

bool TextProcessing::GetVersionFromJSON(
    base::DictionaryValue* dictionary) {
  auto* version_value = dictionary->FindKey("version");
  if (!version_value) {
    return false;
  }
  version_ = std::stoull(version_value->GetString());
  return true;
}

bool TextProcessing::GetTimestampFromJSON(
    base::DictionaryValue* dictionary) {
  auto* timestamp_value = dictionary->FindKey("timestamp");
  if (!timestamp_value) {
    return false;
  }

  timestamp_ = timestamp_value->GetString();
  return true;
}

bool TextProcessing::GetLocaleFromJSON(
    base::DictionaryValue* dictionary) {
  auto* locale_value = dictionary->FindKey("locale");
  if (!locale_value) {
    return false;
  }

  locale_ = locale_value->GetString();
  return true;
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
