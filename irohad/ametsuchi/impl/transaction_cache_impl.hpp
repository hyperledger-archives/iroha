/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_TRANSACTION_CACHE_IMPL_HPP
#define IROHA_TRANSACTION_CACHE_IMPL_HPP

#include "ametsuchi/block_query.hpp"
#include "ametsuchi/transaction_cache.hpp"

namespace iroha {
  namespace ametsuchi {

    class TransactionCacheImpl : public TransactionCache {
     public:
      explicit TransactionCacheImpl(std::unique_ptr<ametsuchi::BlockQuery> block_query);

      bool verify(
          const shared_model::crypto::Hash &transaction_hash) const override;

      bool verify(std::shared_ptr<shared_model::interface::TransactionBatch>
                      batch) const override;

     private:
      std::unique_ptr<ametsuchi::BlockQuery> block_query_;
    };

  }  // namespace transaction_cache
}  // namespace iroha

#endif  // IROHA_TRANSACTION_CACHE_IMPL_HPP
