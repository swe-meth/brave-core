/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bat/ads/internal/ml/pipeline/pipeline_util.h"

namespace ads {
namespace ml {
namespace pipeline {

bool ParseTransformationsJSON(
    base::Value* transformations_value,
    std::vector<transformation::TransformationPtr>& transformations) {
  if (!transformations_value || !transformations_value->is_list()) {
    return false;
  }

  transformations.clear();
  auto transformation_list = transformations_value->GetList();
  for (size_t i = 0; i < transformation_list.size(); i++) {
    const base::Value& transformation = transformation_list[i];

    const std::string* transformation_type =
        transformation.FindStringKey("transformation_type");

    if (!transformation_type) {
      return false;
    }

    std::string parsed_transformation_type = *transformation_type;

    if (parsed_transformation_type.compare("TO_LOWER") == 0) {
      transformations.push_back(std::make_shared<transformation::Lowercase>(
          transformation::Lowercase()));
    }

    if (parsed_transformation_type.compare("NORMALIZE") == 0) {
      transformations.push_back(std::make_shared<transformation::Normalization>(
          transformation::Normalization()));
    }

    if (parsed_transformation_type.compare("HASHED_NGRAMS") == 0) {
      const base::Value* transformation_params =
          transformation.FindKey("params");

      if (!transformation_params) {
        return false;
      }

      const base::Optional<int> nb =
          transformation_params->FindIntKey("num_buckets");
      if (!nb.has_value()) {
        return false;
      }

      int num_buckets = nb.value();

      const base::Value* n_gram_sizes =
          transformation_params->FindListKey("ngrams_range");

      if (!n_gram_sizes) {
        return false;
      }

      std::vector<int> ngram_range;
      auto ngram_range_list = n_gram_sizes->GetList();
      for (size_t i = 0; i < ngram_range_list.size(); i++) {
        const base::Value& n = ngram_range_list[i];
        ngram_range.push_back(n.GetInt());
      }
      transformation::HashedNGrams hashed_ngrams(num_buckets, ngram_range);
      transformations.push_back(
          std::make_shared<transformation::HashedNGrams>(hashed_ngrams));
    }
  }

  return true;
}

bool ParseClassifierJSON(base::Value* classifier_value,
                         model::Linear& linear_model) {
  if (!classifier_value) {
    return false;
  }

  std::vector<std::string> classes;
  std::string* classifier_type =
      classifier_value->FindStringKey("classifier_type");

  if (!classifier_type) {
    return false;
  }

  std::string parsed_classifier_type = *classifier_type;

  if (parsed_classifier_type.compare("LINEAR")) {
    return false;
  }

  base::Value* specified_classes = classifier_value->FindListKey("classes");
  if (!specified_classes) {
    return false;
  }

  auto specified_classes_list = specified_classes->GetList();
  for (size_t i = 0; i < specified_classes_list.size(); i++) {
    const base::Value& class_name = specified_classes_list[i];
    classes.push_back(class_name.GetString());
  }

  base::Value* class_weights = classifier_value->FindDictKey("class_weights");
  if (!class_weights) {
    return false;
  }

  std::map<std::string, data::VectorData> weights;
  for (size_t i = 0; i < classes.size(); i++) {
    base::Value* this_class = class_weights->FindListKey(classes[i]);
    if (!this_class) {
      return false;
    }
    std::vector<double> class_coef_weights;
    auto this_class_list = this_class->GetList();
    for (size_t j = 0; j < this_class_list.size(); j++) {
      const base::Value& weight = this_class_list[j];
      class_coef_weights.push_back(weight.GetDouble());
    }
    weights[classes[i]] = data::VectorData(class_coef_weights);
  }

  std::map<std::string, double> specified_biases;
  base::Value* biases = classifier_value->FindListKey("biases");
  if (!biases) {
    return false;
  }

  auto biases_list = biases->GetList();
  if (biases_list.size() != classes.size()) {
    return false;
  }

  for (size_t i = 0; i < biases_list.size(); i++) {
    const base::Value& this_bias = biases_list[i];
    specified_biases[classes[i]] = this_bias.GetDouble();
  }

  linear_model = model::Linear(weights, specified_biases);
  return true;
}

bool ParsePipelineJSON(
    const std::string& json,
    uint16_t& version,
    std::string& timestamp,
    std::string& locale,
    std::vector<transformation::TransformationPtr>& transformations,
    model::Linear& linear_model) {
  base::Optional<base::Value> root = base::JSONReader::Read(json);

  if (!root) {
    return false;
  }

  base::Optional<int> version_value = root->FindIntKey("version");
  if (!version_value.has_value()) {
    return false;
  }
  int version_number = version_value.value();
  version = static_cast<uint16_t>(version_number);

  std::string* timestamp_value = root->FindStringKey("timestamp");
  if (!timestamp_value) {
    return false;
  }
  timestamp = *timestamp_value;

  std::string* locale_value = root->FindStringKey("locale");
  if (!locale_value) {
    return false;
  }
  locale = *locale_value;

  base::Value* transformations_value = root->FindKey("transformations");
  if (!ParseTransformationsJSON(transformations_value, transformations)) {
    return false;
  }

  base::Value* classifier_value = root->FindKey("classifier");
  if (!ParseClassifierJSON(classifier_value, linear_model)) {
    return false;
  }

  return true;
}

}  // namespace pipeline
}  // namespace ml
}  // namespace ads
