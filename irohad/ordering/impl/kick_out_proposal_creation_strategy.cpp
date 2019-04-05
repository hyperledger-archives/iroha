/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/kick_out_proposal_creation_strategy.hpp"

#include <numeric>
#include "interfaces/common_objects/peer.hpp"

using namespace iroha::ordering;

KickOutProposalCreationStrategy::KickOutProposalCreationStrategy(
    std::shared_ptr<SupermajorityCheckerType> majority_checker)
    : majority_checker_(majority_checker) {}

bool KickOutProposalCreationStrategy::onCollaborationOutcome(
    RoundType round, const PeerList &peers) {
  uint64_t counter = 0;
  std::for_each(last_requested_.begin(),
                last_requested_.end(),
                [&round, &counter](const auto &elem) {
                  if (elem.second >= round) {
                    counter++;
                  }
                });
  auto has_majority =
      majority_checker_->hasMajority(counter, last_requested_.size());
  updateCurrentState(peers);
  return not has_majority;
}

boost::optional<ProposalCreationStrategy::RoundType>
KickOutProposalCreationStrategy::onProposal(PeerType who,
                                            RoundType requested_round) {
  auto iter = last_requested_.find(who);
  if (iter == last_requested_.end()) {
    last_requested_.insert({who, requested_round});
  } else {
    if (iter->second < requested_round) {
      iter->second = requested_round;
    }
  }

  return boost::none;
}

void KickOutProposalCreationStrategy::updateCurrentState(
    const PeerList &peers) {
  last_requested_ =
      std::accumulate(peers.begin(),
                      peers.end(),
                      RoundCollectionType{},
                      [this](auto collection, const auto &peer) {
                        auto iter = last_requested_.find(peer);
                        if (iter != last_requested_.end()) {
                          collection.insert(*iter);
                        } else {
                          collection.insert({peer, RoundType{0, 0}});
                        }
                        return std::move(collection);
                      });
}
