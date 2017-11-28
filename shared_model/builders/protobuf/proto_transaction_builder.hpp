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

#ifndef IROHA_PROTO_TRANSACTION_BUILDER_HPP
#define IROHA_PROTO_TRANSACTION_BUILDER_HPP

#include "block.pb.h"
#include "backend/protobuf/transaction.hpp"
#include "interfaces/common_objects/types.hpp"
#include "amount/amount.hpp"

namespace shared_model {
  namespace proto {
    template <int S = 0>
    class TemplateTransactionBuilder {
     private:
      template <int>
      friend class TemplateTransactionBuilder;

      enum RequiredFields { Command, CreatorAccountId, TxCounter, TOTAL };

      template <int s>
      using NextBuilder = TemplateTransactionBuilder<S | (1 << s)>;

      iroha::protocol::Transaction transaction_;
      iroha::protocol::Transaction::Payload &payload_;

      template <int Sp>
      operator TemplateTransactionBuilder<Sp>&() {
        return reinterpret_cast<TemplateTransactionBuilder<Sp>&>(*this);
      }

     public:
      TemplateTransactionBuilder()
          : payload_(*transaction_.mutable_payload()) {}

      NextBuilder<CreatorAccountId> &creatorAccountId(
          const interface::types::AccountIdType &accountId) {
        payload_.set_creator_account_id(accountId);
        return *this;
      }

      NextBuilder<TxCounter> &txCounter(Transaction::TxCounterType txCounter) {
        payload_.set_tx_counter(txCounter);
        return *this;
      }

      NextBuilder<Command> &addAssetQuantity(
          const interface::types::AccountIdType &accountId,
          const interface::types::AssetIdType &assetId,
          const std::string &amount) {
        auto command = payload_.add_commands()->mutable_add_asset_quantity();
        command->set_account_id(accountId);
        command->set_asset_id(assetId);
        iroha::Amount amt;
        iroha::Amount::createFromString(amount) | [&](auto &&amt) {
          auto proto_amount = command->mutable_amount();
          auto proto_value = proto_amount->mutable_value();
          auto uint64s = amt.to_uint64s();
          proto_value->set_first(uint64s.at(0));
          proto_value->set_second(uint64s.at(1));
          proto_value->set_third(uint64s.at(2));
          proto_value->set_fourth(uint64s.at(3));
          proto_amount->set_precision(amt.getPrecision());
        };
        return *this;
      }

      Transaction build() {
        static_assert(S == (1 << TOTAL) - 1, "Required fields are not set");

        return Transaction(std::move(transaction_));
      }
    };

    using TransactionBuilder = TemplateTransactionBuilder<>;
  }  // namespace proto
}  // namespace shared_model

#endif //IROHA_PROTO_TRANSACTION_BUILDER_HPP
