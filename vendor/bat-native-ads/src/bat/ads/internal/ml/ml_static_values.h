/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_
#define BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_

namespace ads {
namespace ml {

static const int kMinimumWordsToClassify = 20;
static const int kMaximumWordsToClassify = 1234;
static const int kMaximumHtmlLengthToClassify = (1 << 20);
static const int kMaxSubLen = 6;
static const int kNumBuckets = 10000;

}  // namespace ml
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_ML_ML_STATIC_VALUES_H_
