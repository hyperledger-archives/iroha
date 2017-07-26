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

#ifndef IROHA_PB_QUERY_RESPONSE_FACTORY_HPP
#define IROHA_PB_QUERY_RESPONSE_FACTORY_HPP

#include <responses.pb.h>
#include <model/account_asset.hpp>
#include <model/queries/responses/account_assets_response.hpp>
#include <model/queries/responses/account_response.hpp>
#include <nonstd/optional.hpp>
#include "model/queries/responses/error_response.hpp"
#include "model/queries/responses/signatories_response.hpp"
#include "model/queries/responses/transactions_response.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      /**
       * Converting business objects to protobuf and vice versa
       */
      class PbQueryResponseFactory {
       public:
        nonstd::optional<protocol::QueryResponse> serialize(
            const std::shared_ptr<QueryResponse> query_response) const;

        protocol::Account serialize(const model::Account &account) const;
        model::Account deserialize(const protocol::Account &pb_account) const;

        protocol::AccountResponse serialize(
            const model::AccountResponse &accountResponse) const;
        model::AccountResponse deserialize(
            const protocol::AccountResponse pb_response) const;

        protocol::AccountAsset serialize(
            const model::AccountAsset &account_asset) const;
        model::AccountAsset deserialize(
            const protocol::AccountAsset &account_asset) const;

        protocol::AccountAssetResponse serialize(
            const model::AccountAssetResponse &accountAssetResponse) const;
        model::AccountAssetResponse deserialize(
            const protocol::AccountAssetResponse &account_asset_response) const;

        protocol::SignatoriesResponse serialize(
            const model::SignatoriesResponse &signatoriesResponse) const;
        model::SignatoriesResponse deserialize(
            const protocol::SignatoriesResponse &signatoriesResponse) const;

        protocol::TransactionsResponse serialize(
            const model::TransactionsResponse &transactionsResponse)
            const;


        protocol::ErrorResponse serialize(
            const model::ErrorResponse &errorResponse) const;
      };
    }
  }
}

#endif  // IROHA_PB_QUERY_RESPONSE_FACTORY_HPP
