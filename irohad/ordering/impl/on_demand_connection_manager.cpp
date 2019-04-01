/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/on_demand_connection_manager.hpp"

#include <boost/range/combine.hpp>
#include "interfaces/iroha_internal/proposal.hpp"
#include "interfaces/iroha_internal/transaction_batch_impl.hpp"
#include "logger/logger.hpp"
#include "ordering/impl/on_demand_common.hpp"

using namespace iroha;
using namespace iroha::ordering;

OnDemandConnectionManager::OnDemandConnectionManager(
    std::shared_ptr<transport::OdOsNotificationFactory> factory,
    rxcpp::observable<CurrentPeers> peers,
    std::shared_ptr<OrderingGateResendStrategy> batch_resend_strategy,
    logger::LoggerPtr log)
    : log_(std::move(log)),
      factory_(std::move(factory)),
      batch_resend_strategy_(std::move(batch_resend_strategy)),
      subscription_(peers.subscribe([this](const auto &peers) {
        // exclusive lock
        std::lock_guard<std::shared_timed_mutex> lock(mutex_);

        this->initializeConnections(peers);
      })) {}

OnDemandConnectionManager::OnDemandConnectionManager(
    std::shared_ptr<transport::OdOsNotificationFactory> factory,
    rxcpp::observable<CurrentPeers> peers,
    CurrentPeers initial_peers,
    std::shared_ptr<OrderingGateResendStrategy> batch_resend_strategy,
    logger::LoggerPtr log)
    : OnDemandConnectionManager(std::move(factory),
                                peers,
                                std::move(batch_resend_strategy),
                                std::move(log)) {
  // using start_with(initial_peers) results in deadlock
  initializeConnections(initial_peers);
}

OnDemandConnectionManager::~OnDemandConnectionManager() {
  subscription_.unsubscribe();
}

void OnDemandConnectionManager::onBatches(CollectionType batches) {
  std::shared_lock<std::shared_timed_mutex> lock(mutex_);
  /*
   * Transactions are always sent to the round after the next round (+2)
   * There are 4 possibilities - all combinations of commits and rejects in the
   * following two rounds. This can be visualised as a diagram, where: o -
   * current round, x - next round, v - target round
   *
   *    0 1 2         0 1 2         0 1 2         0 1 2
   *  0 o x v       0 o . .       0 o x .       0 o . .
   *  1 . . .       1 x v .       1 v . .       1 x . .
   *  2 . . .       2 . . .       2 . . .       2 v . .
   * RejectReject  CommitReject  RejectCommit  CommitCommit
   */

  CollectionType reject_reject_batches{};
  CollectionType reject_commit_batches{};
  CollectionType commit_reject_batches{};
  CollectionType commit_commit_batches{};

  for (const auto &batch : batches) {
    auto rounds = batch_resend_strategy_->extract(batch);
    auto current_round = batch_resend_strategy_->getCurrentRound();

    if (rounds.find(nextRejectRound(nextRejectRound(current_round)))
        != rounds.end()) {
      reject_reject_batches.push_back(batch);
    }
    if (rounds.find(nextCommitRound(nextRejectRound(current_round)))
        != rounds.end()) {
      reject_commit_batches.push_back(batch);
    }
    if (rounds.find(nextRejectRound(nextCommitRound(current_round)))
        != rounds.end()) {
      commit_reject_batches.push_back(batch);
    }
    if (rounds.find(nextCommitRound(nextCommitRound(current_round)))
        != rounds.end()) {
      commit_commit_batches.push_back(batch);
    }
  }

  connections_.peers[kRejectRejectConsumer]->onBatches(
      std::move(reject_reject_batches));
  connections_.peers[kRejectCommitConsumer]->onBatches(
      std::move(reject_commit_batches));
  connections_.peers[kCommitRejectConsumer]->onBatches(
      std::move(commit_reject_batches));
  connections_.peers[kCommitCommitConsumer]->onBatches(
      std::move(commit_commit_batches));
}

boost::optional<std::shared_ptr<const OnDemandConnectionManager::ProposalType>>
OnDemandConnectionManager::onRequestProposal(consensus::Round round) {
  std::shared_lock<std::shared_timed_mutex> lock(mutex_);

  log_->debug("onRequestProposal, {}", round);

  return connections_.peers[kIssuer]->onRequestProposal(round);
}

void OnDemandConnectionManager::initializeConnections(
    const CurrentPeers &peers) {
  auto create_assign = [this](auto &ptr, auto &peer) {
    ptr = factory_->create(*peer);
  };

  for (auto &&pair : boost::combine(connections_.peers, peers.peers)) {
    create_assign(boost::get<0>(pair), boost::get<1>(pair));
  }
}
