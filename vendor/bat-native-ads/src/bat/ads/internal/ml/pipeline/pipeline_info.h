/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_
#define BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_

#include <string>
#include <vector>

#include "bat/ads/internal/ml/transformation/transformation.h"
#include "bat/ads/internal/ml/model/linear/linear.h"

namespace ads {
namespace ml {
namespace pipeline {

struct PipelineInfo {
  PipelineInfo();

  PipelineInfo(const PipelineInfo& pinfo);

  ~PipelineInfo();

  PipelineInfo(
    const uint16_t& version,
    const std::string& timestamp,
    const std::string& locale,
    const std::vector<transformation::TransformationPtr>& transformations,
    const model::Linear& linear_model);

  uint16_t version;
  std::string timestamp;
  std::string locale;
  std::vector<transformation::TransformationPtr> transformations;
  model::Linear linear_model;
};

}  // namespace pipeline
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_  // NOLINT
