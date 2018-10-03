/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "multi_sig_transactions/mst_notificator_impl.hpp"

#include <functional>

using namespace iroha;

MstNotificatorImpl::MstNotificatorImpl(
    const iroha::MstProcessor &mst_processor,
    std::shared_ptr<iroha::network::PeerCommunicationService> pcs,
    std::shared_ptr<iroha::torii::StatusBus> status_bus,
    std::shared_ptr<shared_model::interface::TxStatusFactory> status_factory)
    : pcs_(std::move(pcs)),
      status_bus_(std::move(status_bus)),
      status_factory_(std::move(status_factory)),
      log_(logger::log("MstNotificator")) {
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
    const MstProcessor::UpdatedStateType &state) {
  log_->info("handleOnStateUpdate: nothing to do");
}

void MstNotificatorImpl::handleOnExpiredBatches(
    const MstProcessor::BatchType &expired_batch) {
  log_->info("handleOnExpiredBatches");
  publishExpiredStatuses(expired_batch->transactions());
}

void MstNotificatorImpl::handleOnCompletedBatches(
    const MstProcessor::BatchType &completed_batch) {
  log_->info("handleOnCompletedBatches");

  publishEnoughSignaturesStatuses(completed_batch->transactions());
  pcs_->propagate_batch(completed_batch);
}

void MstNotificatorImpl::publish(
    const shared_model::interface::types::SharedTxsCollectionType &transactions,
    TxFactoryType::TxStatusFactoryInvoker invoker) {
  std::for_each(transactions.begin(),
                transactions.end(),
                [this, invoker](const auto &tx) {
                  status_bus_->publish((status_factory_.get()->*invoker)(
                      tx->hash(), status_factory_->emptyErrorMassage()));
                });
}

void MstNotificatorImpl::publishEnoughSignaturesStatuses(
    const shared_model::interface::types::SharedTxsCollectionType
        &transactions) {
  publish(transactions, &TxFactoryType::makeEnoughSignaturesCollected);
}

void MstNotificatorImpl::publishExpiredStatuses(
    const shared_model::interface::types::SharedTxsCollectionType
        &transactions) {
  publish(transactions, &TxFactoryType::makeMstExpired);
}
