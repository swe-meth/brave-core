/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <map>
#include <memory>
#include <string>

#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/transformation/normalization.h"

namespace ads {
namespace ml {
namespace transformation {

Normalization::Normalization()
    : Transformation(TransformationType::NORMALIZATION) {}

Normalization::~Normalization() = default;

std::shared_ptr<data::Data> Normalization::Apply(
    const std::shared_ptr<data::Data>& input_data) {
  if (input_data->GetType() != data::DataType::VECTOR_DATA) {
    return std::make_shared<data::VectorData>(
        data::VectorData(0, std::map<unsigned, double>()));
  }

  std::shared_ptr<data::VectorData> vector_data =
      std::static_pointer_cast<data::VectorData>(input_data);

  vector_data->Normalize();
  return vector_data;
}

}  // namespace transformation
}  // namespace ml
}  // namespace ads
