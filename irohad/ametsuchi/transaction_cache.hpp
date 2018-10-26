/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_TRANSACTION_CACHE_HPP
#define IROHA_TRANSACTION_CACHE_HPP

#include "cryptography/hash.hpp"
#include "interfaces/common_objects/transaction_sequence_common.hpp"
#include "interfaces/iroha_internal/transaction_batch.hpp"

namespace iroha {
  namespace ametsuchi {

    /**
     * Persistent cache of transactions, checking if transaction has ever been
     * committed
     */
    class TransactionCache {
     public:
      virtual bool verify(
          const shared_model::crypto::Hash &transaction_hash) const = 0;

      virtual void verify(
          const shared_model::interface::types::BatchesCollectionType::iterator
              &begin,
          const shared_model::interface::types::BatchesCollectionType::iterator
              &end,
          const std::function<
              void(shared_model::interface::types::BatchesCollectionType &&)>
              &valid_txs_processor,
          const std::function<
              void(shared_model::interface::types::BatchesCollectionType &&)>
              &invalid_txs_processor) const = 0;

      virtual bool verify(
          std::shared_ptr<shared_model::interface::TransactionBatch> batch)
          const = 0;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_TRANSACTION_CACHE_HPP
