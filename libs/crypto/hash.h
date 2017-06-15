/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_HASH_H
#define IROHA_HASH_H

extern "C" {
#include <SimpleFIPS202.h>
};

#include <nonstd/optional.hpp>
#include <vector>

namespace iroha {

inline int sha3_224(unsigned char *output, unsigned char *input,
                    size_t in_size) {
  return SHA3_224(output, input, in_size);
}

inline int sha3_256(unsigned char *output, unsigned char *input,
                    size_t in_size) {
  return SHA3_256(output, input, in_size);
}

inline int sha3_384(unsigned char *output, unsigned char *input,
                    size_t in_size) {
  return SHA3_384(output, input, in_size);
}

inline int sha3_512(unsigned char *output, unsigned char *input,
                    size_t in_size) {
  return SHA3_512(output, input, in_size);
}


/*
 * Does not compile. Redefinition of sha3_224. Why?
nonstd::optional<std::vector<uint8_t>> sha3_224(uint8_t *input,
                                                size_t in_size) {
  std::vector<uint8_t> out(224 / 8);
  int res = SHA3_224(out.data(), input, in_size);
  return !res ? nonstd::optional<std::vector<uint8_t>>(out) : nonstd::nullopt;
}


nonstd::optional<std::vector<uint8_t>> sha3_256(uint8_t *input,
                                                size_t in_size) {
  std::vector<uint8_t> out(256 / 8);
  int res = SHA3_256(out.data(), input, in_size);
  return !res ? nonstd::optional<std::vector<uint8_t>>(out) : nonstd::nullopt;
}


nonstd::optional<std::vector<uint8_t>> sha3_384(uint8_t *input,
                                                size_t in_size) {
  std::vector<uint8_t> out(384 / 8);
  int res = SHA3_384(out.data(), input, in_size);
  return !res ? nonstd::optional<std::vector<uint8_t>>(out) : nonstd::nullopt;
}


nonstd::optional<std::vector<uint8_t>> sha3_512(uint8_t *input,
                                                size_t in_size) {
  std::vector<uint8_t> out(512 / 8);
  int res = SHA3_512(out.data(), input, in_size);
  return !res ? nonstd::optional<std::vector<uint8_t>>(out) : nonstd::nullopt;
}
 */

}  // namespace iroha

#endif  // IROHA_HASH_H
