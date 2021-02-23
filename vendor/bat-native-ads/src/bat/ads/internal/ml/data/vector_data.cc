/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <limits>
#include <map>
#include <stdexcept>
#include <vector>

#include "bat/ads/internal/ml/data/vector_data.h"

namespace ads {
namespace ml {
namespace data {

VectorData::VectorData() : Data(DataType::VECTOR_DATA) {}

VectorData::VectorData(const VectorData& vector_data)
    : Data(DataType::VECTOR_DATA) {
  dimension_count_ = vector_data.GetDimensionCount();
  data_ = vector_data.GetRawData();
}

VectorData& VectorData::operator=(const VectorData& vector_data) {
  dimension_count_ = vector_data.GetDimensionCount();
  data_ = vector_data.GetRawData();
  return *this;
}

VectorData::~VectorData() = default;

VectorData::VectorData(int dimension_count,
                       const std::map<unsigned, double>& data)
    : Data(DataType::VECTOR_DATA) {
  dimension_count_ = dimension_count;
  data_.reserve(dimension_count_);
  for (auto data_it = data.begin(); data_it != data.end(); data_it++) {
    data_.push_back(SparseVectorElement(data_it->first, data_it->second));
  }
}

VectorData::VectorData(const std::vector<double>& data)
    : Data(DataType::VECTOR_DATA) {
  dimension_count_ = static_cast<int>(data.size());
  data_.resize(dimension_count_);
  for (int i = 0; i < dimension_count_; ++i) {
    data_[i] = SparseVectorElement(static_cast<unsigned>(i), data[i]);
  }
}

void VectorData::Normalize() {
  double vector_length = 0.0;
  for (size_t i = 0; i < data_.size(); ++i) {
    vector_length += data_[i].second * data_[i].second;
  }
  vector_length = sqrt(vector_length);
  if (vector_length > 1e-7) {
    for (size_t i = 0; i < data_.size(); ++i) {
      data_[i].second /= vector_length;
    }
  }
}

int VectorData::GetDimensionCount() const {
  return dimension_count_;
}

std::vector<SparseVectorElement> VectorData::GetRawData() const {
  return data_;
}

double operator*(const VectorData& a, const VectorData& b) {
  if (!a.dimension_count_ || !b.dimension_count_) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  if (a.dimension_count_ != b.dimension_count_) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  double dot_product = 0.0;
  size_t a_ind = 0;
  size_t b_ind = 0;
  while (a_ind < a.data_.size() && b_ind < b.data_.size()) {
    if (a.data_[a_ind].first == b.data_[b_ind].first) {
      dot_product += a.data_[a_ind].second * b.data_[b_ind].second;
      ++a_ind;
      ++b_ind;
    } else {
      if (a.data_[a_ind].first < b.data_[b_ind].first) {
        ++a_ind;
      } else {
        ++b_ind;
      }
    }
  }

  return dot_product;
}

}  // namespace data
}  // namespace ml
}  // namespace ads
