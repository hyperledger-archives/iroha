/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_TYPES_HPP
#define IROHA_TYPES_HPP

namespace iroha {

  /**
   * std::string is convenient to use but it is not safe.
   * We can not specify the fixed length for string.
   *
   * For std::array it is possible, so we prefer it over std::string.
   */

  template <size_t size>
  using blob_t<size> = std::array<uint8_t, size>;

  template <size_t size>
  using hash_t<size> = blob_t<size>;
  using hash224_t = blob_t<224 / 8>;
  using hash256_t = blob_t<256 / 8>;
  using hash384_t = blob_t<384 / 8>;
  using hash512_t = blob_t<512 / 8>;

  namespace {
    using sig_t = blob_t<64>;  // ed25519 sig is 64 bytes length
    using pubkey_t = blob_t<32>;
    using privkey_t = blob_t<64>;
  }

  // timestamps
  using ts64_t = uint64_t;
  using ts32_t = uint32_t;
}

#endif  // IROHA_TYPES_HPP
