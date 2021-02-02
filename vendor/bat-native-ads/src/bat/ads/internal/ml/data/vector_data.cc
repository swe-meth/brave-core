/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <limits>
#include <map>
#include <vector>
#include <stdexcept>

#include "bat/ads/internal/ml/data/vector_data.h"

namespace ads {
namespace ml {
namespace data {

VectorData::VectorData() : Data(DataType::VECTOR_DATA) {}

VectorData::VectorData(
    const VectorData& vector_data) : Data(DataType::VECTOR_DATA) {
  dimension_count_ = vector_data.GetDimensionCount();
  data_ = vector_data.GetRawData();
}

VectorData& VectorData::operator=(const VectorData& vector_data) {
  dimension_count_ = vector_data.GetDimensionCount();
  data_ = vector_data.GetRawData();
  return *this;
}

VectorData::~VectorData() = default;

VectorData::VectorData(
    int dimension_count,
    const std::map<unsigned, double>& data)
    : Data(DataType::VECTOR_DATA), dimension_count_(dimension_count), data_(data) {}

VectorData::VectorData(
    const std::vector<double>& data) : Data(DataType::VECTOR_DATA) {
  dimension_count_ = static_cast<int>(data.size());
  for (int i = 0; i < dimension_count_; ++i) {
    data_[i] = data[i];
  }
}

void VectorData::Normalize() {
  double vector_length = 0.0;
  for (auto data_it = data_.begin();
      data_it != data_.end();
      data_it++) {
    vector_length += data_it->second * data_it->second;
  }
  vector_length = sqrt(vector_length);
  if (vector_length > 1e-7) {
    for (auto data_it = data_.begin();
        data_it != data_.end();
        data_it++) {
      data_it->second /= vector_length;
    }
  }
}

int VectorData::GetDimensionCount() const {
  return dimension_count_;
}

std::map<unsigned, double> VectorData::GetRawData() const {
  return data_;
}

double operator*(
    const VectorData& a,
    const VectorData& b) {
  if (!a.dimension_count_ || !b.dimension_count_) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  if (a.dimension_count_ != b.dimension_count_) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  double dot_product = 0.0;
  for (auto kv : a.data_) {
    if (b.data_.count(kv.first) > 0) {
      auto tmp = b.data_.at(kv.first);
      dot_product += kv.second * tmp;
    }
  }

  return dot_product;
}

}  // namespace data
}  // namespace ml
}  // namespace ads
