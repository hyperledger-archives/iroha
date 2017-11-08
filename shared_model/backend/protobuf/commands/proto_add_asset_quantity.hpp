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
#include "interfaces/commands/add_asset_quantity.hpp"

namespace shared_model {
  namespace proto {
    class AddAssetQuantity : public interface::AddAssetQuantity {
     public:
      explicit AddAssetQuantity(
          const iroha::protocol::Command &command)
          : command_(command.add_asset_quantity()) {
        auto &value = command_.amount().value();
        amount_ = AmountType(value.first(),
                          value.second(),
                          value.third(),
                          value.fourth(),
                          command_.amount().precision());
      }

      const interface::types::AccountIdType &accountId() const override {
        return command_.account_id();
      }
      const AssetIdType &assetId() const override {
        return command_.asset_id();
      }
      const AmountType &amount() const override {
        return amount_;
      }

      bool operator==(const ModelType &rhs) const override {
        return false;
      }
      ModelType *copy() const override {
        return nullptr;
      }
      OldModelType *makeOldModel() const override {
        return nullptr;
      }

     private:

      const iroha::protocol::AddAssetQuantity &command_;
      AmountType amount_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif //IROHA_PROTO_ADD_ASSET_QUANTITY_HPP
