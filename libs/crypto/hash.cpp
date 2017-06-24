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

#include <common/types.hpp>

extern "C" {
#include <SimpleFIPS202.h>
}

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

  hash224_t sha3_224(uint8_t *input, size_t in_size) {
    hash224_t h;
    SHA3_224(h.begin(), input, in_size);
    return h;
  }

  hash256_t sha3_256(uint8_t *input, size_t in_size) {
    hash256_t h;
    SHA3_256(h.begin(), input, in_size);
    return h;
  }

  hash384_t sha3_384(uint8_t *input, size_t in_size) {
    hash384_t h;
    SHA3_384(h.begin(), input, in_size);
    return h;
  }

  hash512_t sha3_512(uint8_t *input, size_t in_size) {
    hash512_t h;
    SHA3_512(h.begin(), input, in_size);
    return h;
  }

}  // namespace iroha