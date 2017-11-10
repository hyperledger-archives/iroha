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

#ifndef IROHA_PROTO_ADD_ASSET_QUANTITY_HPP
#define IROHA_PROTO_ADD_ASSET_QUANTITY_HPP

#include "commands.pb.h"
#include "cryptography/stub_hash.hpp"
#include "interfaces/commands/add_asset_quantity.hpp"
#include "utils/lazy_initializer.hpp"
#include "backend/protobuf/common_objects/amount.hpp"

namespace shared_model {
  namespace proto {
    class AddAssetQuantity final : public interface::AddAssetQuantity {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit AddAssetQuantity(const iroha::protocol::Command &command)
          : AddAssetQuantity(command.add_asset_quantity()) {
        if (not command.has_add_asset_quantity()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument(
              "Object does not contain add_asset_quantity");
        }
      }

      const interface::types::AccountIdType &accountId() const override {
        return add_asset_quantity_.account_id();
      }

      const interface::types::AssetIdType &assetId() const override {
        return add_asset_quantity_.asset_id();
      }

      const interface::types::AmountType &amount() const override {
        return amount_.get();
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override {
        return new AddAssetQuantity(add_asset_quantity_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit AddAssetQuantity(
          const iroha::protocol::AddAssetQuantity &add_asset_quantity)
          : add_asset_quantity_(add_asset_quantity),
            amount_([this] {
              return proto::Amount(this->add_asset_quantity_.amount());
            }),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }) {}

      // ------------------------------| fields |-------------------------------

      // proto
      const iroha::protocol::AddAssetQuantity add_asset_quantity_;

      // lazy
      Lazy<proto::Amount> amount_;
      Lazy<crypto::StubHash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_ASSET_QUANTITY_HPP
