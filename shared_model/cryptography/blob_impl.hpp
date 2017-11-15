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

#ifndef IROHA_SHARED_MODEL_BLOB_IMPL_HPP
#define IROHA_SHARED_MODEL_BLOB_IMPL_HPP

#include <iomanip>
#include "cryptography/blob.hpp"
#include "utils/lazy_initializer.hpp"

namespace shared_model {
  namespace crypto {
    /**
     * Blob interface implementation.
     */
    class BlobImpl : public Blob {
     public:
      explicit BlobImpl(const std::string &blob)
          : blob_(blob), hex_([this]() {
              std::stringstream ss;
              ss << std::hex << std::setfill('0');
              for (const auto &c : blob_) {
                ss << std::setw(2) << static_cast<int>(c);
              }
              return ss.str();
            }) {}

      const std::string &blob() const override { return blob_; }

      const std::string &hex() const override { return hex_.get(); }

      size_t size() const override { return blob_.size(); }

      BlobImpl *copy() const override { return new BlobImpl(blob()); };

     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      std::string blob_;
      Lazy<std::string> hex_;
    };
  }  // namespace crypto
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_BLOB_IMPL_HPP
