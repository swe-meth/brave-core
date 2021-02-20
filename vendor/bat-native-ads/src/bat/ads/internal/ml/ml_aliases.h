/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_ML_ALIASES_H_
#define BAT_ADS_INTERNAL_ML_ML_ALIASES_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {

using PredictionMap = std::map<std::string, data::VectorData>;
using TransformationPtr = std::shared_ptr<transformation::Transformation>;
using TransformationVector = std::vector<TransformationPtr>;

}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_ML_ALIASES_H_
