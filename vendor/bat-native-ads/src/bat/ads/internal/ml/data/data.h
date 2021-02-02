/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_DATA_DATA_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_DATA_DATA_H_  // NOLINT

#include <map>
#include <string>
#include <vector>

namespace ads {
namespace ml {
namespace data {

enum class DataType {
  TEXT_DATA = 0,
  VECTOR_DATA = 1
};

class Data {
 public:
  Data(const DataType& type);

  virtual ~Data();

  DataType GetType() const;

 protected:
  const DataType type_;
};

}  // namespace data
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_DATA_DATA_H_  // NOLINT
