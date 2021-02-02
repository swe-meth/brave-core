/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_  // NOLINT
#define BAT_ADS_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_  // NOLINT

#include <map>
#include <string>
#include <vector>

namespace ads {
namespace ml {
namespace transformation {

class HashVectorizer {
 public:
  HashVectorizer(const HashVectorizer& other);

  HashVectorizer(int n_buckets, const std::vector<int>& subgrams);

  ~HashVectorizer();

  std::map<unsigned, double> GetFrequencies(const std::string& html);

  std::vector<unsigned> GetSubstringSizes() const;

  int GetBucketCount() const;

 private:
  int GetHash(const std::string& text);

  std::vector<unsigned> substring_sizes_;
  int bucket_count_;
};

}  // namespace transformation
}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_  // NOLINT
