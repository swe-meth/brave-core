/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_DATA_VECTOR_DATA_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_DATA_VECTOR_DATA_H_  // NOLINT

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bat/ads/internal/ml/data/data.h"

typedef std::pair<unsigned, double> SparseVectorElement;

namespace ads {
namespace ml {
namespace data {

class VectorData : public Data {
 public:
  VectorData();

  VectorData(const VectorData& vector_data);

  explicit VectorData(const std::vector<double>& data);

  VectorData& operator=(const VectorData& vector_data);

  VectorData(int dimension_count, const std::map<unsigned, double>& data);

  ~VectorData() override;

  friend double operator*(const VectorData& a, const VectorData& b);

  void Normalize();

  int GetDimensionCount() const;

  std::vector<SparseVectorElement> GetRawData() const;

 private:
  int dimension_count_;
  std::vector<SparseVectorElement> data_;
};

}  // namespace data
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_DATA_VECTOR_DATA_H_  // NOLINT
