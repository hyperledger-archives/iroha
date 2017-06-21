/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_BLOCK_HPP
#define IROHA_BLOCK_HPP

#include <common.hpp>
#include <vector>
#include "singature.hpp"
#include "transaction.hpp"

namespace iroha {
  namespace dao {

    /**
     * Block is DAO-structure that provide all block-related information
     */
    struct Block {

      /**
       * Calculated as sha3_256(META + BODY fields)
       * HEADER field
       */
      hash256_t hash;

      /**
       * List of signatures
       * HEADER field
       */
      std::vector<Signature> sigs;

      /**
       * Creation timestamp
       * HEADER field
       */
      ts64_t created_ts;

      /**
       * Number of blocks in ledger
       * META field
       */
      uint64_t height;

      /**
       * Hash of previous block
       * META field
       */
      hash256_t prev_hash;

      /**
       * Number of transactions
       * META field
       */
      uint16_t tx_number;

      /**
       * Root of merkle tree based on block and ledger
       * META field
       */
      hash256_t merkle_root;

      /**
       * Attached transactions
       * BODY field
       */
      std::vector<Transaction> transactions;
    };
  }
}

#endif  // IROHA_BLOCK_HPP
