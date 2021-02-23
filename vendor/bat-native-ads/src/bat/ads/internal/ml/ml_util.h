/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_ML_UTIL_H_
#define BAT_ADS_INTERNAL_ML_ML_UTIL_H_

#include <cmath>
#include <map>
#include <memory>
#include <string>

#include "bat/ads/internal/ml/ml_aliases.h"
#include "bat/ads/internal/ml/transformation/hashed_ngrams.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"
#include "bat/ads/internal/ml/transformation/normalization.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {

std::map<std::string, double> Softmax(const std::map<std::string, double>& y);

TransformationPtr GetTransformationCopy(const TransformationPtr& tr_ptr);

TransformationVector GetTransformationVectorCopy(
    const TransformationVector& tr_vect);

}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_ML_UTIL_H_
