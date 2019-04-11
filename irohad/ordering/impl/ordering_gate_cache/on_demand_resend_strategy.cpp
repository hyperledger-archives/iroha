/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/ordering_gate_cache/on_demand_resend_strategy.hpp"

#include <boost/assert.hpp>
#include <iostream>
#include "interfaces/iroha_internal/transaction_batch.hpp"
#include "interfaces/transaction.hpp"
#include "ordering/impl/on_demand_common.hpp"

using namespace iroha::ordering;

bool OnDemandResendStrategy::feed(
    std::shared_ptr<shared_model::interface::TransactionBatch> batch) {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  auto res =
      sent_batches_.emplace(batch, std::make_pair(current_round_, false));
  return res.second;
}

bool OnDemandResendStrategy::readyToUse(
    std::shared_ptr<shared_model::interface::TransactionBatch> batch) {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  auto batch_found = sent_batches_.find(batch);
  if (batch_found != sent_batches_.end()) {
    batch_found->second.second = true;
    return true;
  }
  return false;
}

OnDemandResendStrategy::RoundSetType OnDemandResendStrategy::extract(
    std::shared_ptr<shared_model::interface::TransactionBatch> batch) {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  auto reachable_from_current = reachableInTwoRounds(current_round_);

  auto saved_round_iterator = sent_batches_.find(batch);
  if (saved_round_iterator == sent_batches_.end()) {
    return reachable_from_current;
  }
  if (not saved_round_iterator->second.second) {
    return reachable_from_current;
  }

  auto saved_round = saved_round_iterator->second.first;
  saved_round_iterator->second.first = current_round_;

  auto reachable_from_saved = reachableInTwoRounds(saved_round);
  RoundSetType target_rounds{};
  std::set_difference(reachable_from_current.begin(),
                      reachable_from_current.end(),
                      reachable_from_saved.begin(),
                      reachable_from_saved.end(),
                      std::inserter(target_rounds, target_rounds.begin()));
  return target_rounds;
}

void OnDemandResendStrategy::remove(
    const cache::OrderingGateCache::HashesSetType &hashes) {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  for (auto it = sent_batches_.begin(); it != sent_batches_.end();) {
    cache::OrderingGateCache::HashesSetType restored_hashes{};
    for (auto tx : it->first->transactions()) {
      restored_hashes.insert(tx->hash());
    }
    if (hashes == restored_hashes) {
      sent_batches_.erase(it);
      return;
    } else {
      ++it;
    }
  }
}

void OnDemandResendStrategy::setCurrentRound(
    const iroha::consensus::Round &current_round) {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  current_round_ = current_round;
}

iroha::consensus::Round OnDemandResendStrategy::getCurrentRound() const {
  std::shared_lock<std::shared_timed_mutex> lock(access_mutex_);
  return current_round_;
}

OnDemandResendStrategy::RoundSetType
OnDemandResendStrategy::reachableInTwoRounds(
    const consensus::Round &round) const {
  RoundSetType reachable_rounds{};
  reachable_rounds.insert(nextCommitRound(nextCommitRound(round)));
  reachable_rounds.insert(nextCommitRound(nextRejectRound(round)));
  reachable_rounds.insert(nextRejectRound(nextCommitRound(round)));
  reachable_rounds.insert(nextRejectRound(nextRejectRound(round)));
  return reachable_rounds;
}
