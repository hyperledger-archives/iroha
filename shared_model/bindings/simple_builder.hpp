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
#include "builders/protobuf/unsigned_proto.hpp"

namespace shared_model {
  namespace bindings {
    /**
     * Wrapper class for transaction builder. Designed only for SWIG bindings,
     * don't use it other cases.
     */
    class SimpleBuilder {
     public:
      SimpleBuilder() = default;

      /**
       * Creates new SimpleBuilder
       * @param o - TemplateTransactionBuilder<> instance
       */
      SimpleBuilder(
          const proto::TemplateTransactionBuilder<
              (1 << shared_model::proto::TemplateTransactionBuilder<>::total)
              - 1> &o);

      /**
       * Append creator account id field to builder
       * @param account_id - account id
       * @return builder with account id field appended
       */
      SimpleBuilder creatorAccountId(
          const interface::types::AccountIdType &account_id);

      /**
       * Append tx counter field to builder
       * @param tx_counter - tx counter
       * @return builder with tx counter field appended
       */
      SimpleBuilder txCounter(uint64_t tx_counter);

      /**
       * Append add asset quantity field to builder
       * @param account_id - account id
       * @param asset_id - asset id
       * @param amount - amount
       * @return builder with add asset quantity field appended
       */
      SimpleBuilder addAssetQuantity(
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
