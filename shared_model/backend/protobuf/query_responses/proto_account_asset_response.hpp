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

#ifndef IROHA_PROTO_ACCOUNT_ASSET_RESPONSE_HPP
#define IROHA_PROTO_ACCOUNT_ASSET_RESPONSE_HPP

#include "backend/protobuf/common_objects/proto_account_asset.hpp"
#include "interfaces/query_responses/account_asset_response.hpp"
#include "responses.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class AccountAssetResponse final : public interface::AccountAssetResponse {
     public:
      template <typename QueryResponseType>
      explicit AccountAssetResponse(QueryResponseType &&queryResponse)
          : queryResponse_(std::forward<QueryResponseType>(queryResponse)),
            accountAssetResponse_(
                [this] { return queryResponse_->account_assets_response(); }),
            accountAsset_([this] {
              return AccountAsset(accountAssetResponse_->account_asset());
            }) {}

      AccountAssetResponse(const AccountAssetResponse &accountAssetResponse)
          : AccountAssetResponse(*accountAssetResponse.queryResponse_) {}

      AccountAssetResponse(AccountAssetResponse &&accountAssetResponse)
          : AccountAssetResponse(
                std::move(accountAssetResponse.queryResponse_.variant())) {}

      const interface::AccountAsset &accountAsset() const override {
        return *accountAsset_;
      }

      ModelType *copy() const override {
        return new AccountAssetResponse(
            iroha::protocol::QueryResponse(*queryResponse_));
      }

     private:
      detail::ReferenceHolder<iroha::protocol::QueryResponse> queryResponse_;

      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<const iroha::protocol::AccountAssetResponse &>
          accountAssetResponse_;

      const Lazy<AccountAsset> accountAsset_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ACCOUNT_ASSET_RESPONSE_HPP
