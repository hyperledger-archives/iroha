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

#ifndef IROHA_PROTO_AMOUNT_HPP
#define IROHA_PROTO_AMOUNT_HPP

#include <numeric>
#include "cryptography/stub_hash.hpp"
#include "interfaces/common_objects/amount.hpp"
#include "primitive.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class Amount final : public interface::Amount {
     private:
      using RefAmount = detail::ReferenceHolder<iroha::protocol::Amount>;

     public:
      explicit Amount(const iroha::protocol::Amount *amount)
          : Amount(RefAmount(amount)) {}

      explicit Amount(iroha::protocol::Amount amount)
          : Amount(RefAmount(std::move(amount))) {}

      const boost::multiprecision::uint256_t &intValue() const override {
        return *multiprecision_repr_;
      }

      interface::types::PrecisionType precision() const override {
        return *precision_;
      }

      const HashType &hash() const override { return *hash_; }

      Amount *copy() const override { return new Amount(*proto_amount_); }

     private:
      explicit Amount(RefAmount &&ref)
          : proto_amount_(std::move(ref)),
            multiprecision_repr_([this] {
              const auto offset = 64u;
              auto times = 3u;
              boost::multiprecision::uint256_t result;
              result |= proto_amount_->value().first() << offset * times--;
              result |= proto_amount_->value().second() << offset * times--;
              result |= proto_amount_->value().third() << offset * times--;
              result |= proto_amount_->value().fourth() << offset * times--;
              return result;
            }),
            precision_([this] { return proto_amount_->precision(); }),
            hash_([this] { return crypto::StubHash(); }) {}

      // proto
      RefAmount proto_amount_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      // lazy
      Lazy<boost::multiprecision::uint256_t> multiprecision_repr_;
      Lazy<interface::types::PrecisionType> precision_;
      Lazy<crypto::StubHash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_AMOUNT_HPP
