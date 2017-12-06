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

#include "bindings/model_builder.hpp"

namespace shared_model {
  namespace bindings {
    ModelBuilder ModelBuilder::creatorAccountId(
        const interface::types::AccountIdType &account_id) {
      return ModelBuilder(builder_.creatorAccountId(account_id));
    }

    ModelBuilder ModelBuilder::txCounter(uint64_t tx_counter) {
      return ModelBuilder(builder_.txCounter(tx_counter));
    }

    ModelBuilder ModelBuilder::addAssetQuantity(
        const interface::types::AccountIdType &account_id,
        const interface::types::AssetIdType &asset_id,
        const std::string &amount) {
      return ModelBuilder(builder_.assetQuantity(account_id, asset_id, amount));
    }

    proto::UnsignedWrapper<proto::Transaction> ModelBuilder::build() {
      return builder_.build();
    }
  }  // namespace bindings
}  // namespace shared_model
