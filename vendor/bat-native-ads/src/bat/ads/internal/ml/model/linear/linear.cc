/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bat/ads/internal/ml/ml_util.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/model/linear/linear.h"

namespace ads {
namespace ml {
namespace model {

Linear::Linear() {}

Linear::Linear(const std::map<std::string, data::VectorData>& weights,
               const std::map<std::string, double>& biases) {
  weights_ = weights;
  biases_ = biases;
}

Linear::Linear(const Linear& linear_model) = default;

Linear::~Linear() = default;

std::map<std::string, double> Linear::Predict(const data::VectorData& x) {
  std::map<std::string, double> rtn;
  for (auto kv : weights_) {
    rtn[kv.first] = kv.second * x + biases_[kv.first];
  }
  return rtn;
}

std::map<std::string, double> Linear::TopPredictions(const data::VectorData& x,
                                                     int top_count) {
  std::map<std::string, double> pred_map = Predict(x);
  std::map<std::string, double> pred_map_softmax = Softmax(pred_map);
  std::vector<std::pair<double, std::string>> pred_order;
  pred_order.reserve(pred_map_softmax.size());
  for (auto pred_it = pred_map_softmax.begin();
       pred_it != pred_map_softmax.end(); pred_it++) {
    pred_order.push_back(std::make_pair(pred_it->second, pred_it->first));
  }
  std::sort(pred_order.rbegin(), pred_order.rend());
  std::map<std::string, double> top_pred;
  if (top_count > 0) {
    pred_order.resize(top_count);
  }
  for (size_t i = 0; i < pred_order.size(); ++i) {
    top_pred[pred_order[i].second] = pred_order[i].first;
  }
  return top_pred;
}

}  // namespace model
}  // namespace ml
}  // namespace ads
