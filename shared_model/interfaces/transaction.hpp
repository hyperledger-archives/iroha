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
#include "interfaces/common_objects/signature.hpp"
#include "interfaces/primitive.hpp"
#include "model/transaction.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Transaction class represent well-formed intent from client to change
     * state of ledger.
     */
    class Transaction
        : public Primitive<Transaction, iroha::model::Transaction> {
     public:
      /// Type of transaction signature
      using SignatureType = Signature;

      /// Type of set of signatures
      using SignatureSetType = std::unordered_set<const SignatureType>;

      // TODO discuss adding signatures

      /**
       * @return attached signatures
       */
      virtual const SignatureSetType &signatures() const = 0;

      /// Type of timestamp
      using TimestampType = uint64_t;

      /**
       * @return time of creation
       */
      virtual const TimestampType &created_time() const = 0;

      /// Type of creator id
      using CreatorIdType = std::string;

      /**
       * @return creator of transaction
       */
      virtual const CreatorIdType &creator_account_id() const = 0;

      /// Type of counter
      using TxCounterType = uint64_t;

      /**
       * @return actual number of transaction of this user
       */
      virtual TxCounterType transaction_counter() const = 0;

      /// Type of command
      using CommandType = Command;

      /// Type of ordered collection of commands
      using CommnadsType = std::vector<Command>;

      /**
       * @return attached commands
       */
      virtual CommnadsType &commands() const = 0;

      /// Quorum type
      using QuorumType = uint8_t;
      /**
       * @return quorum of transaction.
       * Quorum means how much signatures of account required for performing
       * transaction.
       */
      virtual const QuorumType &quorum() const;

      /// Type of hash
      using HashType = Hash;

      /**
       * @return hash of transaction.
       * Equality of hashes means equality of transactions.
       * Signatures are not affected on hash
       */
      virtual const HashType &hash() const;

      /**
       * Equality of transactions means equality of hashes only.
       * This invariant useful for checking transaction in fast way.
       * @param rhs - other transaction.
       * @return true if hashes equal.
       */
      bool operator==(const Transaction &rhs) const override {
        return this->hash() == rhs.hash();
      }

      iroha::model::Transaction *makeOldModel() const {
        // TODO implement conversion to old style transaction
        return nullptr;
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_TRANSACTION_HPP
