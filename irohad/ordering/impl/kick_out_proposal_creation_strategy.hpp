/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_KICK_OUT_PROPOSAL_CREATION_STRATEGY_HPP
#define IROHA_KICK_OUT_PROPOSAL_CREATION_STRATEGY_HPP

#include "ordering/ordering_service_proposal_creation_strategy.hpp"

#include <memory>
#include <unordered_map>
#include "common/pointer_utils.hpp"
#include "consensus/round.hpp"
#include "consensus/yac/supermajority_checker.hpp"

namespace iroha {
  namespace ordering {

    class KickOutProposalCreationStrategy : public ProposalCreationStrategy {
     public:
      using SupermajorityCheckerType =
          iroha::consensus::yac::SupermajorityChecker;
      KickOutProposalCreationStrategy(
          std::shared_ptr<SupermajorityCheckerType>);

      bool onCollaborationOutcome(RoundType round,
                                  const PeerList &peers) override;

      boost::optional<RoundType> onProposal(PeerType who,
                                            RoundType requested_round) override;

     private:
      using RoundCollectionType = std::unordered_map<
          PeerType,
          RoundType,
          DereferenceHash<PeerType, shared_model::interface::Peer::PeerHash>>;

      /**
       * Update peers state with new peers.
       * Note: the method removes peers which are not participating in consensus
       * and adds new with minimal round
       * @param peers - list of peers which fetched in the last round
       */
      void updateCurrentState(const PeerList &peers);

      std::shared_ptr<SupermajorityCheckerType> majority_checker_;
      RoundCollectionType last_requested_;
    };
  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_KICK_OUT_PROPOSAL_CREATION_STRATEGY_HPP
