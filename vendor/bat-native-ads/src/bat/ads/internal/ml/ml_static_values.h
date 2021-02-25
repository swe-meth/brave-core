/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_

namespace ads {
namespace ml {

const int kMinimumWordsToClassify = 20;
const int kMaximumWordsToClassify = 1234;
const int kMaximumHtmlLengthToClassify = (1 << 20);
const int kMaxSubLen = 6;
const int kNumBuckets = 10000;

}  // namespace ml
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_
