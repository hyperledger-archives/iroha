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

#ifndef IROHA_SHARED_MODEL_STUB_HASH_HPP
#define IROHA_SHARED_MODEL_STUB_HASH_HPP

#include "cryptography/hash.hpp"

namespace shared_model {
  namespace crypto {

    class StubHash : public Hash {
     public:
      StubHash() {}
      StubHash(const StubHash &) {}
      StubHash(StubHash &&) {}

      StubHash &operator=(const StubHash &) { return *this; }
      StubHash &operator=(StubHash &&) { return *this; }

      const std::string &blob() const override { return string; }

      const std::string &hex() const override { return string; }

      size_t size() const override { return 0; }

      ModelType *copy() const override { return new StubHash; }

      const std::string string = "";
    };
  }  // namespace crypto
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_STUB_HASH_HPP
