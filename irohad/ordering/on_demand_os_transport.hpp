/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ON_DEMAND_OS_TRANSPORT_HPP
#define IROHA_ON_DEMAND_OS_TRANSPORT_HPP

#include <memory>
#include <utility>
#include <vector>

#include <boost/optional.hpp>
#include "consensus/round.hpp"

namespace shared_model {
  namespace interface {
    class TransactionBatch;
    class Proposal;
    class Peer;
  }  // namespace interface
}  // namespace shared_model

namespace iroha {
  namespace ordering {
    namespace transport {

      namespace details {

        /**
         * Type of peer identity
         */
        using PeerType = shared_model::interface::Peer;

        /**
         * Type of stored proposals
         */
        using ProposalType = shared_model::interface::Proposal;

        /**
         * Type of stored transaction batches
         */
        using TransactionBatchType =
            std::shared_ptr<shared_model::interface::TransactionBatch>;

        /**
         * Type of inserted collections
         */
        using CollectionType = std::vector<TransactionBatchType>;
      }  // namespace details

      /**
       * Notification interface of on demand ordering service.
       */
      class OdOsNotification {
       public:
        using ProposalType = details::ProposalType;
        using TransactionBatchType = details::TransactionBatchType;
        using CollectionType = details::CollectionType;

        /**
         * Callback on receiving transactions
         * @param batches - vector of passed transaction batches
         */
        virtual void onBatches(CollectionType batches) = 0;

        /**
         * Callback on request about proposal
         * @param round - number of collaboration round.
         * Calculated as block_height + 1
         * @return proposal for requested round
         */
        virtual boost::optional<std::shared_ptr<const ProposalType>>
        onRequestProposal(consensus::Round round) = 0;

        virtual ~OdOsNotification() = default;
      };

      /**
       * Factory for creating communication interface to a specific peer
       */
      class OdOsNotificationFactory {
       public:
        /**
         * Create corresponding OdOsNotification interface for peer
         * Returned pointer is guaranteed to be not equal to nullptr
         * @param peer - peer to connect
         * @return connection represented with OdOsNotification interface
         */
        virtual std::unique_ptr<OdOsNotification> create(
            const details::PeerType &to) = 0;

        virtual ~OdOsNotificationFactory() = default;
      };

      class OdNotificationOsSide {
       public:
        using InitiatorPeerType = std::shared_ptr<details::PeerType>;
        using ProposalType = details::ProposalType;
        using TransactionBatchType = details::TransactionBatchType;
        using CollectionType = details::CollectionType;

        /**
         * Callback on receiving transactions
         * @param batches - vector of passed transaction batches
         * @param from - peer which sends batches
         */
        virtual void onBatches(CollectionType batches,
                               InitiatorPeerType from) = 0;

        /**
         * Callback on request about proposal
         * @param round - number of collaboration round.
         * Calculated as block_height + 1
         * @param requester - peer which requests proposal
         * @return proposal for requested round
         */
        virtual boost::optional<std::shared_ptr<const ProposalType>>
        onRequestProposal(consensus::Round round,
                          InitiatorPeerType requester) = 0;

        virtual ~OdNotificationOsSide() = default;
      };

    }  // namespace transport
  }    // namespace ordering
}  // namespace iroha

#endif  // IROHA_ON_DEMAND_OS_TRANSPORT_HPP
