/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/ml_static_values.h"
#include "bat/ads/internal/ml/transformation/hash_vectorizer.h"
#include "third_party/zlib/zlib.h"

namespace ads {
namespace ml {
namespace transformation {

HashVectorizer::HashVectorizer() {
  for (int i = 1; i <= kMaxSubLen; ++i) {
    substring_sizes_.push_back(i);
  }
  bucket_count_ = kNumBuckets;
}

HashVectorizer::~HashVectorizer() = default;

HashVectorizer::HashVectorizer(int bucket_count,
                               const std::vector<int>& subgrams) {
  for (size_t i = 0; i < subgrams.size(); i++) {
    substring_sizes_.push_back(subgrams[i]);
  }
  bucket_count_ = bucket_count;
}

HashVectorizer::HashVectorizer(const HashVectorizer& hash_vectorizer) {
  bucket_count_ = hash_vectorizer.GetBucketCount();
  substring_sizes_ = hash_vectorizer.GetSubstringSizes();
}

std::vector<unsigned> HashVectorizer::GetSubstringSizes() const {
  return substring_sizes_;
}

int HashVectorizer::GetBucketCount() const {
  return bucket_count_;
}

unsigned HashVectorizer::GetHash(const std::string& substring) {
  const auto* u8str = substring.c_str();
  auto rtn =
      crc32(crc32(0L, Z_NULL, 0), reinterpret_cast<const unsigned char*>(u8str),
            strlen(u8str));
  return rtn;
}

std::map<unsigned, double> HashVectorizer::GetFrequencies(
    const std::string& html) {
  std::string data = html;
  std::map<unsigned, double> frequencies;
  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify);
  }
  // get hashes of substrings for each of the substring lengths defined:
  for (auto const& substring_size : substring_sizes_) {
    if (substring_size > data.length()) {
      break;
    }
    for (size_t i = 0; i < data.length() - substring_size + 1; ++i) {
      std::string ss = data.substr(i, substring_size);
      unsigned idx = GetHash(ss);
      ++frequencies[idx % static_cast<unsigned>(bucket_count_)];
    }
  }
  return frequencies;
}

}  // namespace transformation
}  // namespace ml
}  // namespace ads
