/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MST_NOTIFICATOR_IMPL_HPP
#define IROHA_MST_NOTIFICATOR_IMPL_HPP

#include "multi_sig_transactions/mst_notificator.hpp"

#include <memory>

#include "interfaces/iroha_internal/tx_status_factory.hpp"
#include "logger/logger.hpp"
#include "multi_sig_transactions/mst_processor.hpp"
#include "network/peer_communication_service.hpp"
#include "torii/status_bus.hpp"

namespace iroha {
  class MstNotificatorImpl : public MstNotificator {
   public:
    // -----------------------------| public API|-------------------------------

    MstNotificatorImpl(
        const iroha::MstProcessor &mst_processor,
        std::shared_ptr<iroha::network::PeerCommunicationService> pcs,
        std::shared_ptr<iroha::torii::StatusBus> status_bus,
        std::shared_ptr<shared_model::interface::TxStatusFactory>
            status_factory);

    void handleOnStateUpdate(
        const MstProcessor::UpdatedStateType &state) override;

    void handleOnExpiredBatches(
        const MstProcessor::BatchType &expired_batch) override;

    void handleOnCompletedBatches(
        const MstProcessor::BatchType &batch) override;

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

    std::shared_ptr<iroha::network::PeerCommunicationService> pcs_;

    std::shared_ptr<iroha::torii::StatusBus> status_bus_;

    std::shared_ptr<shared_model::interface::TxStatusFactory> status_factory_;

    logger::Logger log_;
  };
}  // namespace iroha
#endif  // IROHA_MST_NOTIFICATOR_IMPL_HPP
