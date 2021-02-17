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

std::shared_ptr<data::Data> Lowercase::Apply(
    const std::shared_ptr<data::Data>& input_data) {
  if (input_data->GetType() != data::DataType::TEXT_DATA) {
    return std::make_shared<data::Data>(data::TextData(""));
  }

  const std::shared_ptr<data::TextData> text_data =
      std::static_pointer_cast<data::TextData>(input_data);

  std::string lowercase_text = text_data->GetText();

  std::transform(lowercase_text.begin(), lowercase_text.end(),
                 lowercase_text.begin(), ::tolower);

  return std::make_shared<data::TextData>(data::TextData(lowercase_text));
}

}  // namespace transformation
}  // namespace ml
}  // namespace ads
