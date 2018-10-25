/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_TRANSACTION_CACHE_HPP
#define IROHA_TRANSACTION_CACHE_HPP

#include "cryptography/hash.hpp"
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
      virtual bool verify(
          std::shared_ptr<shared_model::interface::TransactionBatch> batch)
          const = 0;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_TRANSACTION_CACHE_HPP
