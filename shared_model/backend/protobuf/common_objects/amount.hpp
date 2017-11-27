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
#include "interfaces/common_objects/amount.hpp"
#include "primitive.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class Amount final : public interface::Amount {
     public:
      template <typename AmountType>
      explicit Amount(AmountType &&amount)
          : proto_amount_(std::forward<AmountType>(amount)),
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
            blob_([this] {
              return BlobType(proto_amount_->SerializeAsString());
            }) {}

      Amount(const Amount &o) : Amount(*o.proto_amount_) {}

      Amount(Amount &&o) noexcept
          : Amount(std::move(o.proto_amount_.variant())) {}

      const boost::multiprecision::uint256_t &intValue() const override {
        return *multiprecision_repr_;
      }

      interface::types::PrecisionType precision() const override {
        return *precision_;
      }

      const BlobType &blob() const override { return *blob_; }

      Amount *copy() const override {
        return new Amount(iroha::protocol::Amount(*proto_amount_));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Amount> proto_amount_;

      // lazy
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<boost::multiprecision::uint256_t> multiprecision_repr_;

      const Lazy<interface::types::PrecisionType> precision_;

      const Lazy<BlobType> blob_;
    };

  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_AMOUNT_HPP
