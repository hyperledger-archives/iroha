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

namespace shared_model {
  namespace proto {
    class AddAssetQuantity final : public interface::AddAssetQuantity {
     private:
      template<typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit AddAssetQuantity(const iroha::protocol::Command &command)
          : AddAssetQuantity(command.add_asset_quantity()) {}

      const interface::types::AccountIdType &accountId() const override {
        return add_asset_quantity_.account_id();
      }

      const interface::types::AssetIdType &assetId() const override {
        return add_asset_quantity_.asset_id();
      }

      const interface::types::AmountType &amount() const override {
        return lazy_amount_.get();
      }

      const HashType &hash() const override { return *lazy_hash_.get(); }

      ModelType *copy() const override {
        return new AddAssetQuantity(add_asset_quantity_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit AddAssetQuantity(
          const iroha::protocol::AddAssetQuantity &add_asset_quantity)
          : add_asset_quantity_(add_asset_quantity),
            lazy_amount_([this] {
              auto &value = this->add_asset_quantity_.amount().value();
              return interface::types::AmountType(
                  value.first(),
                  value.second(),
                  value.third(),
                  value.fourth(),
                  add_asset_quantity_.amount().precision());
            }),
            lazy_hash_([this] {
              // TODO 10/11/2017 muratovv repace with effective implementation
              return std::make_shared<crypto::StubHash>();
            }) {}

      // ------------------------------| fields |-------------------------------

      // proto
      const iroha::protocol::AddAssetQuantity add_asset_quantity_;

      // lazy
      Lazy<interface::types::AmountType> lazy_amount_;
      Lazy<std::shared_ptr<crypto::Hash>> lazy_hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_ASSET_QUANTITY_HPP
