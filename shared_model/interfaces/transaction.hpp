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

#ifndef IROHA_SHARED_MODEL_TRANSACTION_HPP
#define IROHA_SHARED_MODEL_TRANSACTION_HPP

#include <unordered_set>
#include <vector>
#include "interfaces/commands/command.hpp"
#include "interfaces/common_objects/hash.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/primitive.hpp"
#include "interfaces/signable.hpp"
#include "model/transaction.hpp"
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Transaction class represent well-formed intent from client to change
     * state of ledger.
     */
    class Transaction
        : public Signable<Transaction, iroha::model::Transaction> {
     public:
      /// Type of creator id
      using CreatorIdType = std::string;

      /**
       * @return creator of transaction
       */
      virtual const types::AccountIdType &creatorAccountId() const = 0;

      /// Type of counter
      using TxCounterType = uint64_t;

      /**
       * @return actual number of transaction of this user
       */
      virtual TxCounterType transactionCounter() const = 0;

      /// Type of command
      using CommandType = detail::PolymorphicWrapper<Command>;

      /// Type of ordered collection of commands
      using CommandsType = std::vector<CommandType>;

      /**
       * @return attached commands
       */
      virtual const CommandsType &commands() const = 0;

      /// Quorum type
      using QuorumType = uint8_t;
      /**
       * @return quorum of transaction.
       * Quorum means how much signatures of account required for performing
       * transaction.
       */
      virtual const QuorumType &quorum() const = 0;

      iroha::model::Transaction *makeOldModel() const {
        iroha::model::Transaction *oldStyleTransaction =
            new iroha::model::Transaction();
        oldStyleTransaction->created_ts = createdTime();
        oldStyleTransaction->creator_account_id = creatorAccountId();
        oldStyleTransaction->tx_counter = transactionCounter();

        std::for_each(commands().begin(),
                      commands().end(),
                      [oldStyleTransaction](auto &command) {
                        oldStyleTransaction->commands.emplace_back(
                            std::shared_ptr<iroha::model::Command>(
                                command->makeOldModel()));
                      });

        std::for_each(signatures().begin(),
                      signatures().end(),
                      [oldStyleTransaction](auto &sig) {
                        oldStyleTransaction->signatures.emplace_back(
                            *sig->makeOldModel());
                      });

        return oldStyleTransaction;
      }

      std::string toString() const {
        util::PrettyStringBuilder builder;
        builder.initString("Transaction");
        builder.appendField("hash", hash().hex());
        builder.appendField("txCounter", std::to_string(transactionCounter()));
        builder.appendField("creatorAccountId", creatorAccountId());
        builder.appendField("quorum", std::to_string(quorum()));
        builder.appendField("createdTime", std::to_string(createdTime()));
        builder.appendField("commands");
        builder.insertLevel();
        std::for_each(
            commands().begin(), commands().end(), [&builder](auto &command) {
              builder.appendField(command->toString());
            });
        builder.removeLevel();
        builder.appendField("signatures");
        builder.insertLevel();
        std::for_each(
            signatures().begin(), signatures().end(), [&builder](auto &sig) {
              builder.appendField(sig->toString());
            });
        builder.removeLevel();
        builder.finalizeString();
        return builder.getResult();
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_TRANSACTION_HPP
