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

#include "builders/protobuf/transaction.hpp"
#include "builders/protobuf/unsigned_proto.hpp"

namespace shared_model {
  namespace bindings {
    /**
     * Wrapper class for transaction builder. Designed only for SWIG bindings,
     * don't use in other cases.
     */
    class ModelBuilder {
     private:
      template <int Sp>
      explicit ModelBuilder(const proto::TemplateTransactionBuilder<Sp> &o)
          : builder_(o) {}

     public:
      ModelBuilder() = default;

      /**
       * Sets id of account creator
       * @param account_id - account id
       * @return builder with account_id field appended
       */
      ModelBuilder creatorAccountId(
          const interface::types::AccountIdType &account_id);

      /**
       * Sets transaction counter field
       * @param tx_counter - transaction counter
       * @return builder with tx_counter field appended
       */
      ModelBuilder txCounter(uint64_t tx_counter);

      /**
       * Adds given quantity of given asset to account
       * @param account_id - account id
       * @param asset_id - asset id
       * @param amount - amount of asset to add
       * @return builder with added given quantity of given asset for account
       */
      ModelBuilder addAssetQuantity(
          const interface::types::AccountIdType &account_id,
          const interface::types::AssetIdType &asset_id,
          const std::string &amount);

      /**
       * Builds result with all appended fields
       * @return wrapper on unsigned transaction
       */
      proto::UnsignedWrapper<proto::Transaction> build();

     private:
      proto::TemplateTransactionBuilder<
          (1 << shared_model::proto::TemplateTransactionBuilder<>::total) - 1>
          builder_;
    };
  }  // namespace bindings
}  // namespace shared_model

#endif  // IROHA_SIMPLE_BUILDER_HPP
