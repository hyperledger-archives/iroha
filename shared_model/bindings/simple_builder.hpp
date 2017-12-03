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

#ifndef IROHA_SIMPLE_BUILDER_HPP
#define IROHA_SIMPLE_BUILDER_HPP

#include "builders/protobuf/proto_transaction_builder.hpp"

namespace shared_model {
  namespace proto {
    class SimpleBuilder {
     public:
      SimpleBuilder() = default;

      SimpleBuilder(const TransactionBuilder &o);

      SimpleBuilder creatorAccountId(
          const interface::types::AccountIdType &account_id);

      SimpleBuilder txCounter(uint64_t tx_counter);

      SimpleBuilder addAssetQuantity(
          const interface::types::AccountIdType &account_id,
          const interface::types::AssetIdType &asset_id,
          const std::string &amount);

      Transaction build();

     private:
      TransactionBuilder builder_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_SIMPLE_BUILDER_HPP
