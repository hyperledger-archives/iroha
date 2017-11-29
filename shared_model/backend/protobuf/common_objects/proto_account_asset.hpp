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

#ifndef IROHA_PROTO_ACCOUNT_ASSET_HPP
#define IROHA_PROTO_ACCOUNT_ASSET_HPP

#include "backend/protobuf/common_objects/proto_amount.hpp"
#include "interfaces/common_objects/account_asset.hpp"
#include "responses.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class AccountAsset final : public interface::AccountAsset {
     public:
      template <typename AccountAssetType>
      explicit AccountAsset(AccountAssetType &&accountAssetType)
          : protoAccountAsset_(
                std::forward<AccountAssetType>(accountAssetType)),
            accountId_([this] { return protoAccountAsset_->account_id(); }),
            assetId_([this] { return protoAccountAsset_->asset_id(); }),
            balance_([this] { return Amount(protoAccountAsset_->balance()); }),
            blob_([this] {
              return BlobType(protoAccountAsset_->SerializeAsString());
            }) {}

      AccountAsset(const AccountAsset &o)
          : AccountAsset(*o.protoAccountAsset_) {}

      AccountAsset(AccountAsset &&o) noexcept
          : AccountAsset(std::move(o.protoAccountAsset_.variant())) {}

      const interface::types::AccountIdType &accountId() const override {
        return *accountId_;
      }

      const interface::types::AssetIdType &assetId() const override {
        return *assetId_;
      }

      const interface::Amount &balance() const override { return *balance_; }

      const BlobType &blob() const override { return *blob_; }

      AccountAsset *copy() const override {
        return new AccountAsset(
            iroha::protocol::AccountAsset(*protoAccountAsset_));
      }

     private:
      detail::ReferenceHolder<iroha::protocol::AccountAsset> protoAccountAsset_;

      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<interface::types::AccountIdType> accountId_;

      const Lazy<interface::types::AssetIdType> assetId_;

      const Lazy<Amount> balance_;

      const Lazy<BlobType> blob_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ACCOUNT_ASSET_HPP
