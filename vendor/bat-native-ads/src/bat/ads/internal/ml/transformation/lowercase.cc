/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <memory>
#include <string>

#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/transformation/lowercase.h"

namespace ads {
namespace ml {
namespace transformation {

Lowercase::Lowercase() : Transformation(TransformationType::LOWERCASE) {}

Lowercase::~Lowercase() = default;

std::unique_ptr<data::Data> Lowercase::Apply(
    const std::unique_ptr<data::Data>& input_data) {
  if (input_data->GetType() != data::DataType::TEXT_DATA) {
    return std::make_unique<data::Data>(data::TextData(""));
  }

  data::TextData* text_data = static_cast<data::TextData*>(input_data.get());

  std::string lowercase_text = text_data->GetText();

  std::transform(lowercase_text.begin(), lowercase_text.end(),
                 lowercase_text.begin(), ::tolower);

  return std::make_unique<data::TextData>(data::TextData(lowercase_text));
}

}  // namespace transformation
}  // namespace ml
}  // namespace ads