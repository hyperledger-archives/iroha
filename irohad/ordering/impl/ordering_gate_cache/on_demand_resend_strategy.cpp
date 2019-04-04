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
  std::set<consensus::Round> valid_rounds{
      nextCommitRound(nextCommitRound(current_round_)),
      nextCommitRound(nextRejectRound(current_round_)),
      nextRejectRound(nextCommitRound(current_round_)),
      nextRejectRound(nextRejectRound(current_round_))};

  auto saved_round_iterator = sent_batches_.find(batch);
  if (saved_round_iterator == sent_batches_.end()) {
    return valid_rounds;
  }
  if (not saved_round_iterator->second.second) {
    return valid_rounds;
  }
  auto saved_round = saved_round_iterator->second.first;

  if (nextCommitRound(nextCommitRound(saved_round)) == current_round_) {
    // do nothing
  } else if (nextRejectRound(nextCommitRound(saved_round)) == current_round_) {
    valid_rounds.erase(nextCommitRound(nextRejectRound(current_round_)));
  } else if (nextCommitRound(nextRejectRound(saved_round)) == current_round_) {
    valid_rounds.erase(nextCommitRound(nextRejectRound(current_round_)));
  } else if (nextRejectRound(nextRejectRound(saved_round)) == current_round_) {
    valid_rounds.erase(nextCommitRound(nextRejectRound(current_round_)));
    valid_rounds.erase(nextRejectRound(nextCommitRound(current_round_)));
    valid_rounds.erase(nextCommitRound(nextCommitRound(current_round_)));
  } else {
    // do nothing
  }

  saved_round_iterator->second.first = current_round_;

  return valid_rounds;
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
