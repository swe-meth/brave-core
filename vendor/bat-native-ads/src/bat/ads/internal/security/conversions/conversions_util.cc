/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/security/conversions/conversions_util.h"

#include <vector>

#include "base/base64.h"
#include "base/json/json_reader.h"
#include "base/rand_util.h"
#include "base/strings/string_number_conversions.h"
#include "bat/ads/internal/conversions/verifiable_conversion_info.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/security/conversions/verifiable_conversion_envelope_info.h"
#include "bat/ads/internal/security/crypto_util.h"
#include "bat/ads/internal/security/key_pair_info.h"
#include "bat/ads/internal/string_util.h"

namespace ads {
namespace security {

namespace {

const char kAlgorithm[] = "x25519-xsalsa20-poly1305";
const size_t kCryptoBoxZeroBytes = 16;       // crypto_box_BOXZEROBYTES
const size_t kCryptoBoxPublicKeyBytes = 32;  // crypto_box_PUBLICKEYBYTES
const size_t kVacCipherTextLength = 32;
const size_t kVacMessageMaxLength = 30;
const size_t kVacMessageMinLength = 15;

std::vector<uint8_t> Base64ToBytes(const std::string& value_base64) {
  std::string value_as_string;
  base::Base64Decode(value_base64, &value_as_string);

  const std::string hex_encoded =
      base::HexEncode(value_as_string.data(), value_as_string.size());

  std::vector<uint8_t> bytes;
  base::HexStringToBytes(hex_encoded, &bytes);

  return bytes;
}

}  // namespace

base::Optional<VerifiableConversionEnvelopeInfo> EncryptAndEncode(
    const VerifiableConversionInfo& verifiable_conversion) {
  const std::string message = verifiable_conversion.id;
  const std::string public_key_base64 = verifiable_conversion.public_key;

  if (message.length() < kVacMessageMinLength ||
      message.length() > kVacMessageMaxLength) {
    return base::nullopt;
  }

  if (!IsLatinAlphaNumeric(message)) {
    return base::nullopt;
  }

  // Protocol requires at least 2 trailing zero-padding bytes
  std::vector<uint8_t> plaintext(message.begin(), message.end());
  plaintext.insert(plaintext.end(), kVacCipherTextLength - plaintext.size(), 0);
  DCHECK_EQ(kVacCipherTextLength, plaintext.size());

  const std::vector<uint8_t> public_key = Base64ToBytes(public_key_base64);
  if (public_key.size() != kCryptoBoxPublicKeyBytes) {
    return base::nullopt;
  }

  const KeyPairInfo ephemeral_key_pair = GenerateBoxKeyPair();

  const std::vector<uint8_t> nonce = GenerateNonce();

  const std::vector<uint8_t> padded_ciphertext =
      Encrypt(plaintext, nonce, public_key, ephemeral_key_pair.secret_key);

  // The receiving TweetNaCl.js client does not require padding
  const std::vector<uint8_t> ciphertext(
      padded_ciphertext.begin() + kCryptoBoxZeroBytes, padded_ciphertext.end());

  VerifiableConversionEnvelopeInfo envelope;
  envelope.algorithm = kAlgorithm;
  envelope.ciphertext = base::Base64Encode(ciphertext);
  envelope.ephemeral_public_key =
      base::Base64Encode(ephemeral_key_pair.public_key);
  envelope.nonce = base::Base64Encode(nonce);

  if (!envelope.IsValid()) {
    return base::nullopt;
  }

  return envelope;
}

std::string DecodeAndDecrypt(const VerifiableConversionEnvelopeInfo envelope,
                             const std::string& advertiser_secret_key_base64) {
  std::string message;
  if (!envelope.IsValid()) {
    return message;
  }

  std::vector<uint8_t> advertiser_secret_key =
      Base64ToBytes(advertiser_secret_key_base64);
  std::vector<uint8_t> nonce = Base64ToBytes(envelope.nonce);
  std::vector<uint8_t> ciphertext = Base64ToBytes(envelope.ciphertext);
  std::vector<uint8_t> ephemeral_public_key =
      Base64ToBytes(envelope.ephemeral_public_key);

  // API requires 16 leading zero-padding bytes
  ciphertext.insert(ciphertext.begin(), kCryptoBoxZeroBytes, 0);

  std::vector<uint8_t> plaintext =
      Decrypt(ciphertext, nonce, ephemeral_public_key, advertiser_secret_key);
  message = (const char*)&plaintext.front();

  return message;
}

}  // namespace security
}  // namespace ads
