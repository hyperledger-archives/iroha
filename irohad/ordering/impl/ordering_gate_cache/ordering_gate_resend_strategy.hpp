/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ORDERING_GATE_RESEND_STRATEGY_HPP
#define IROHA_ORDERING_GATE_RESEND_STRATEGY_HPP

#include <set>
#include "consensus/round.hpp"
#include "ordering/impl/ordering_gate_cache/ordering_gate_cache.hpp"

namespace iroha {
  namespace ordering {
    /**
     * Strategy for batch resend interface
     */
    class OrderingGateResendStrategy {
     public:
      /**
       * Inserts new batch into strategy
       * @param batch - batch to insert
       * @return true if operation success
       */
      virtual bool feed(
          std::shared_ptr<shared_model::interface::TransactionBatch> batch) = 0;

      /**
       * Get inserted before batch ready to extract
       * @param batch - batch to get ready
       * @return false if batch doesn't exist
       */
      virtual bool readyToUse(
          std::shared_ptr<shared_model::interface::TransactionBatch> batch) = 0;

      using RoundSetType = std::set<consensus::Round>;

      /**
       * Returns collection of interested future rounds for inserted and ready
       * batch
       * @param batch - batch to examine
       */
      virtual RoundSetType extract(
          std::shared_ptr<shared_model::interface::TransactionBatch> batch) = 0;

      /**
       * Removes batch with given hashes
       * @param hashes to remove batch with
       */
      virtual void remove(
          const cache::OrderingGateCache::HashesSetType &hashes) = 0;

      /**
       * Sets current round to a given one
       */
      virtual void setCurrentRound(const consensus::Round &current_round) = 0;

      /**
       * Returns saved round
       */
      virtual consensus::Round getCurrentRound() const = 0;
    };

  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_ORDERING_GATE_RESEND_STRATEGY_HPP
