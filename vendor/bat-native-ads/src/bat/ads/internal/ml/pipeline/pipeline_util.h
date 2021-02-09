/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_  // NOLINT

#include <vector>

#include "base/json/json_reader.h"
#include "base/values.h"
#include "bat/ads/internal/ml/model/linear/linear.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/transformation/normalization.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {
namespace pipeline {

bool ParseTransformationsJSON(
    base::Value* transformations_value,
    std::vector<transformation::TransformationPtr>& transformations);

bool ParseClassifierJSON(base::Value* classifier_value,
                         model::Linear& linear_model);

bool ParsePipelineJSON(
    const std::string& json,
    uint16_t& version,
    std::string& timestamp,
    std::string& locale,
    std::vector<transformation::TransformationPtr>& transformations,
    model::Linear& linear_model);

}  // namespace pipeline
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_  // NOLINT
