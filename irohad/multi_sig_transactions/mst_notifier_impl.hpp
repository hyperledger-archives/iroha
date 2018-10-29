/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MST_NOTIFICATOR_IMPL_HPP
#define IROHA_MST_NOTIFICATOR_IMPL_HPP

#include "multi_sig_transactions/mst_notifier.hpp"

#include <memory>

#include "common/subscription_manager.hpp"
#include "interfaces/iroha_internal/transaction_sequence.hpp"
#include "interfaces/iroha_internal/tx_status_factory.hpp"
#include "logger/logger.hpp"
#include "multi_sig_transactions/mst_processor.hpp"
#include "network/peer_communication_service.hpp"
#include "torii/status_bus.hpp"

namespace iroha {
  class MstNotifierImpl : public MstNotifier,
                          private iroha::utils::SubscriptionManager {
   public:
    // -----------------------------| public API|-------------------------------

    MstNotifierImpl(
        std::shared_ptr<iroha::MstProcessor> mst_processor,
        std::shared_ptr<iroha::network::PeerCommunicationService> pcs,
        std::shared_ptr<iroha::torii::StatusBus> status_bus,
        std::shared_ptr<shared_model::interface::TxStatusFactory>
            status_factory);

    void handleStateUpdate(
        const MstProcessor::UpdatedStateType &state) override;

    void handleExpiredBatches(
        const MstProcessor::BatchType &expired_batch) override;

    void handleCompletedBatches(const MstProcessor::BatchType &batch) override;

   private:
    // ----------------------------| private API|-------------------------------

    /// shortcut for \class shared_model::interface::TxStatusFactory type
    using TxFactoryType = shared_model::interface::TxStatusFactory;

    /**
     * Publish a bunch of statuses for corresponding transactions
     * @param invoker - pointer to factory method which creates corresponding
     * status
     */
    void publish(const shared_model::interface::types::SharedTxsCollectionType
                     &transactions,
                 TxFactoryType::TxStatusFactoryInvoker invoker);

    /**
     * Publish EnoughSignatures status for passed transactions
     */
    void publishEnoughSignaturesStatuses(
        const shared_model::interface::types::SharedTxsCollectionType
            &transactions);

    /**
     * Publish Expired status for passed transactions
     */
    void publishExpiredStatuses(
        const shared_model::interface::types::SharedTxsCollectionType
            &transactions);

    /**
     * Publish PendingMst status for passed transactions
     */
    void publishPendingStatuses(
        const shared_model::interface::types::SharedTxsCollectionType
            &transactions);

    std::shared_ptr<iroha::network::PeerCommunicationService> pcs_;

    std::shared_ptr<iroha::torii::StatusBus> status_bus_;

    std::shared_ptr<shared_model::interface::TxStatusFactory> status_factory_;

    logger::Logger log_;
  };
}  // namespace iroha
#endif  // IROHA_MST_NOTIFICATOR_IMPL_HPP
