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

#ifndef IROHA_SHARED_MODEL_ACCOUNT_ASSETS_RESPONSE_HPP
#define IROHA_SHARED_MODEL_ACCOUNT_ASSETS_RESPONSE_HPP

#include "interfaces/primitive.hpp"
#include "model/account_asset.hpp"  // TODO 27/10/2017 muratovv replace with shared_model account_asset
#include "model/queries/responses/account_assets_response.hpp"

namespace shared_model {
  namespace interface {
    class AccountAssetResponse
        : public Primitive<AccountAssetResponse,
                           iroha::model::AccountAssetResponse> {
     public:
      virtual const iroha::model::AccountAsset &accountAsset() = 0;

      // TODO 27/10/2017 muratovv implement available primitive methods
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_ACCOUNT_ASSETS_RESPONSE_HPP
