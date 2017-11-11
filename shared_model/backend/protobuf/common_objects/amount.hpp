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

namespace shared_model {
  namespace proto {
    class Amount final : public interface::Amount {
     public:
      Amount(const iroha::protocol::Amount &proto_amount)
          : multiprecision_repr([this] {
              const auto offset = 64u;
              std::vector<uint64_t> result;
              result.push_back(proto_amount_.value().first());
              result.push_back(proto_amount_.value().second());
              result.push_back(proto_amount_.value().third());
              result.push_back(proto_amount_.value().fourth());

              // last concat not requires shift
              return std::accumulate(result.begin(),
                                     result.end() - 1,
                                     boost::multiprecision::uint256_t(),
                                     [&offset](auto &res, auto &num) {
                                       return (res | num) << offset;
                                     })
                  | *(result.end() - 1);
            }),
            precision_([this] { return proto_amount_.precision(); }),
            hash_([this] { return crypto::StubHash(); }) {}

      const boost::multiprecision::uint256_t &intValue() const override {
        return multiprecision_repr.get();
      }

      uint8_t precision() const override { return precision_.get(); }

      const HashType &hash() const override { return hash_.get(); }

      Amount *copy() const override { return new Amount(proto_amount_); }

     private:
      // proto
      const iroha::protocol::Amount proto_amount_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      // lazy
      Lazy<const boost::multiprecision::uint256_t> multiprecision_repr;
      Lazy<uint8_t> precision_;
      Lazy<crypto::StubHash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_AMOUNT_HPP
