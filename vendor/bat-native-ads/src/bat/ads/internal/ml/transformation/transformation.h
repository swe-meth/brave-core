/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_  // NOLINT

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "bat/ads/internal/ml/data/data.h"

namespace ads {
namespace ml {
namespace transformation {

enum class TransformationType {
  LOWERCASE = 0,
  HASHED_NGRAMS = 1,
  NORMALIZATION = 2
};

class Transformation {
 public:
  Transformation(const TransformationType& type);

  Transformation(
    const Transformation& t);

  virtual ~Transformation();

  TransformationType GetType() const;

  virtual std::shared_ptr<data::Data> Get(
      const std::shared_ptr<data::Data>& input_data) = 0;

 protected:
  const TransformationType type_;
};

typedef std::shared_ptr<Transformation> TransformationPtr;

}  // namespace transformation
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_  // NOLINT
