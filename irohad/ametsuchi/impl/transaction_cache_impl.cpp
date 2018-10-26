/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ametsuchi/impl/transaction_cache_impl.hpp"

namespace iroha {
  namespace ametsuchi {

    TransactionCacheImpl::TransactionCacheImpl(
        std::unique_ptr<ametsuchi::BlockQuery> block_query)
        : block_query_(std::move(block_query)) {}

    bool TransactionCacheImpl::verify(
        const shared_model::crypto::Hash &transaction_hash) const {
      return block_query_->hasTxWithHash(transaction_hash);
    }

    bool TransactionCacheImpl::verify(
        std::shared_ptr<shared_model::interface::TransactionBatch> batch)
        const {
      const auto &batch_transactions = batch->transactions();
      return std::all_of(
          batch_transactions.begin(),
          batch_transactions.end(),
          [this](std::shared_ptr<shared_model::interface::Transaction> tx) {
            return block_query_->hasTxWithHash(tx->hash());
          });
    }

    void TransactionCacheImpl::verify(
        const shared_model::interface::types::BatchesCollectionType::iterator
            &begin,
        const shared_model::interface::types::BatchesCollectionType::iterator
            &end,
        const std::function<
            void(shared_model::interface::types::BatchesCollectionType &&)>
            &valid_txs_processor,
        const std::function<
            void(shared_model::interface::types::BatchesCollectionType &&)>
            &invalid_txs_processor) const {

      auto it = std::partition(
          begin, end, [this](auto batch) { return verify(batch); });
      valid_txs_processor(
          shared_model::interface::types::BatchesCollectionType(begin, it));
      invalid_txs_processor(
          shared_model::interface::types::BatchesCollectionType(it, end));
    }
  }  // namespace ametsuchi
}  // namespace iroha
