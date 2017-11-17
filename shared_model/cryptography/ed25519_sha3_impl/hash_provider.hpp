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

#ifndef IROHA_HASH_PROVIDER_HPP
#define IROHA_HASH_PROVIDER_HPP

#include "cryptography/blob.hpp"
#include "cryptography/hash.hpp"

namespace shared_model {
  namespace crypto {
    /**
     * Wrapper class for hashing.
     */
    class HashProvider {
     public:
      /**
       * Hash with sha3-256
       * @param blob - blob to hash
       * @return Hash of provided blob
       */
      Hash sha3_256(const Blob &blob) const;

      /**
       * Hash with sha3-512
       * @param blob - blob to hash
       * @return Hash of provided blob
       */
      Hash sha3_512(const Blob &blob) const;
    };
  }  // namespace crypto
}  // namespace shared_model

#endif  // IROHA_HASH_PROVIDER_HPP
