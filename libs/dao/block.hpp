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

    struct Block {
      // HEADER

      // calculated as sha3_256(meta)
      // meta includes all fields except hash, sigs, created_ts, transactions
      hash256_t hash;

      // array of signatures
      std::vector<Signature> sigs;

      // created timestamp
      ts64_t created_ts;

      // META
      // current id = ledger version = number of block in a ledger
      uint64_t height;
      hash256_t prev_hash;    // hash of previous block
      uint16_t tx_number;     // number of transactions
      hash256_t merkle_root;  // global merkle root

      // BODY
      std::vector<Transaction> transactions;
    };
  }
}

#endif  // IROHA_BLOCK_HPP
