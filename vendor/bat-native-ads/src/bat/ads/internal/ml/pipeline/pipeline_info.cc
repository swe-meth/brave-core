/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <vector>

#include "bat/ads/internal/ml/pipeline/pipeline_info.h"

namespace ads {
namespace ml {
namespace pipeline {

PipelineInfo::PipelineInfo() = default;

PipelineInfo::PipelineInfo(const PipelineInfo& pinfo) = default;

PipelineInfo::~PipelineInfo() = default;

PipelineInfo::PipelineInfo(
    const uint16_t& version,
    const std::string& timestamp,
    const std::string& locale,
    const TransformationVector& transformations,
    const model::Linear& linear_model)
    : version(version),
      timestamp(timestamp),
      locale(locale),
      transformations(transformations),
      linear_model(linear_model) {}

}  // namespace pipeline
}  // namespace ml
}  // namespace ads
