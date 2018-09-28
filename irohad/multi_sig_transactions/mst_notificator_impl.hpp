/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MST_NOTIFICATOR_IMPL_HPP
#define IROHA_MST_NOTIFICATOR_IMPL_HPP

#include "multi_sig_transactions/mst_notificator.hpp"

#include "multi_sig_transactions/mst_processor.hpp"
#include "network/peer_communication_service.hpp"

#include <memory>

namespace iroha {
  class MstNotificatorImpl : public MstNotificator {
   public:
    // -----------------------------| public API|-------------------------------

    MstNotificatorImpl(
        const iroha::MstProcessor &mst_processor,
        std::shared_ptr<iroha::network::PeerCommunicationService> pcs);

    void handleOnStateUpdate(
        const MstProcessor::UpdatedStateType &state) override;

    void handleOnExpiredBatches(
        const MstProcessor::BatchType &expired_batch) override;

    void handleOnCompletedBatches(
        const MstProcessor::BatchType &batch) override;

   private:
    // ----------------------------| private API|-------------------------------

    std::shared_ptr<iroha::network::PeerCommunicationService> pcs_;
  };
}  // namespace iroha
#endif  // IROHA_MST_NOTIFICATOR_IMPL_HPP
