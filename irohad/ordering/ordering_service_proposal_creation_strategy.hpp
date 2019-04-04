/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ORDERING_SERVICE_PROPOSAL_CREATION_STRATEGY_HPP
#define IROHA_ORDERING_SERVICE_PROPOSAL_CREATION_STRATEGY_HPP

#include <memory>
#include <vector>

#include <boost/optional.hpp>
#include "consensus/round.hpp"
#include "interfaces/common_objects/peer.hpp"

namespace iroha {
  namespace ordering {

    /**
     * Class provides a strategy for creation proposals regarding to new rounds
     * and requests from other peers
     */
    class ProposalCreationStrategy {
     public:
      /// shortcut for peer type
      using PeerType = std::shared_ptr<shared_model::interface::Peer>;
      /// shortcut for round type
      using RoundType = consensus::Round;
      /// collection of peers type
      using PeerList = std::vector<PeerType>;

      /**
       * Indicates the start of new round and provide information about proposal
       * creation
       * @param round - new consensus round
       * @param peers - peers which participate in new round
       * @return true, if proposal should be created in new round
       */
      virtual bool onCollaborationOutcome(RoundType round,
                                          const PeerList &peers) = 0;

      /**
       * Notify the strategy about proposal request
       * @param who - peer which requests the proposal
       * @param requested_round - in which round proposal is requested
       * @return round where proposal is required to be created immediately
       */
      virtual boost::optional<RoundType> onProposal(
          PeerType who, RoundType requested_round) = 0;

      virtual ~ProposalCreationStrategy() = default;
    };
  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_ORDERING_SERVICE_PROPOSAL_CREATION_STRATEGY_HPP
