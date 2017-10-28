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

#ifndef IROHA_SHARED_MODEL_BLOCK_HPP
#define IROHA_SHARED_MODEL_BLOCK_HPP

#include "interfaces/signable.hpp"
#include "interfaces/transaction.hpp"
#include "model/block.hpp"

namespace shared_model {
  namespace interface {

    class Block : public Signable<Block, iroha::model::Block> {
     public:
      /// Block number (height) type
      using BlockHeightType = uint64_t;

      /**
       * @return block number in the ledger
       */
      virtual BlockHeightType &height() const = 0;

      /**
       * @return hash of a previous block
       */
      virtual HashType &prevHash() const = 0;

      /// Type of a number of transactions in block
      using TransactionsNumberType = uint16_t;

      /**
       * @return amount of transactions in block
       */
      virtual TransactionsNumberType &txsNumber() const = 0;

      /**
       * @return Root of merkle tree based on the block and all previous blocks
       * in the ledger
       */
      virtual HashType &merkleRoot() const = 0;

      /// Type of transactions' collection
      using TransactionsType = std::vector<Transaction>;

      /**
       * @return collection of transactions
       */
      virtual TransactionsType &transactions() const = 0;

      iroha::model::Block *makeOldModel() const {
        iroha::model::Block *oldStyleBlock = new iroha::model::Block();
        oldStyleBlock->height = height();
        oldStyleBlock->prev_hash = prevHash();
        oldStyleBlock->txs_number = txsNumber();
        oldStyleBlock->merkle_root = merkleRoot();
        for (auto &tx : transactions()) {
          oldStyleBlock->transactions.push_back(*tx.makeOldModel());
        }
        oldStyleBlock->created_ts = createdTime();
        oldStyleBlock->hash = hash();
        for (auto &sig : signatures()) {
          oldStyleBlock->sigs.push_back(sig);
        }
        return oldStyleBlock;
      }

      std::string toString() const {
        std::string result("Block: [");
        result += "hash=" + hash().hex() + ", ";
        result += "height=" + height() + ", ";
        result += "prevHash=" + prevHash().hex() + ", ";
        result += "txsNumber=" + txsNumber() + ", ";
        result += "merkleRoot=" + merkleRoot().hex() + ", ";
        result += "created time=" + createdTime() + ", ";
        result += "transactions=[";
        for (auto &tx : transactions()) {
          result += tx.toString() + " ";
        }
        result += "], ";
        result += "signatures=[";
        for (auto &sig : signatures()) {
          result += sig.toString() + " ";
        }
        result += "]]";
        return result;
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_BLOCK_HPP
