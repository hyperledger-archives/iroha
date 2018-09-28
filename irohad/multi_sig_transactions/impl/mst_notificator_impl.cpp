/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "multi_sig_transactions/mst_notificator_impl.hpp"

using namespace iroha;

MstNotificatorImpl::MstNotificatorImpl(
    const iroha::MstProcessor &mst_processor,
    std::shared_ptr<iroha::network::PeerCommunicationService> pcs)
    : pcs_(std::move(pcs)) {
  mst_processor.onStateUpdate().subscribe(
      [this](const auto &state) { handleOnStateUpdate(state); });
  mst_processor.onExpiredBatches().subscribe([this](const auto &expired_batch) {
    handleOnExpiredBatches(expired_batch);
  });
  mst_processor.onPreparedBatches().subscribe(
      [this](const auto &completed_batch) {
        handleOnCompletedBatches(completed_batch);
      });
}

void MstNotificatorImpl::handleOnStateUpdate(
    const MstProcessor::UpdatedStateType &state) {}

void MstNotificatorImpl::handleOnExpiredBatches(
    const MstProcessor::BatchType &expired_batch) {}

void MstNotificatorImpl::handleOnCompletedBatches(
    const MstProcessor::BatchType &batch) {}
