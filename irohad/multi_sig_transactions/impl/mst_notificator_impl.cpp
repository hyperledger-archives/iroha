/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "multi_sig_transactions/mst_notificator_impl.hpp"
#include "interfaces/iroha_internal/transaction_batch.hpp"
#include "interfaces/transaction.hpp"

#include <functional>

using namespace iroha;

MstNotificatorImpl::MstNotificatorImpl(
    std::shared_ptr<iroha::MstProcessor> mst_processor,
    std::shared_ptr<iroha::network::PeerCommunicationService> pcs,
    std::shared_ptr<iroha::torii::StatusBus> status_bus,
    std::shared_ptr<shared_model::interface::TxStatusFactory> status_factory)
    : pcs_(std::move(pcs)),
      status_bus_(std::move(status_bus)),
      status_factory_(std::move(status_factory)),
      log_(logger::log("MstNotificator")) {
  addSubscription(mst_processor->onStateUpdate().subscribe(
      [this](const auto &state) { this->handleOnStateUpdate(state); }));

  addSubscription(mst_processor->onExpiredBatches().subscribe(
      [this](const auto &expired_batch) {
        this->handleOnExpiredBatches(expired_batch);
      }));

  addSubscription(mst_processor->onPreparedBatches().subscribe(
      [this](const auto &completed_batch) {
        this->handleOnCompletedBatches(completed_batch);
      }));
}

void MstNotificatorImpl::handleOnStateUpdate(
    const MstProcessor::UpdatedStateType &state) {
  log_->info("handleOnStateUpdate");
  std::for_each(state->getBatches().begin(),
                state->getBatches().end(),
                [this](const auto &updated_batch) {
                  this->publishPendingStatuses(updated_batch->transactions());
                });
}

void MstNotificatorImpl::handleOnExpiredBatches(
    const MstProcessor::BatchType &expired_batch) {
  log_->info("handleOnExpiredBatches");
  publishExpiredStatuses(expired_batch->transactions());
}

void MstNotificatorImpl::handleOnCompletedBatches(
    const MstProcessor::BatchType &completed_batch) {
  log_->info("handleOnCompletedBatches");

  std::for_each(
      completed_batch->transactions().begin(),
      completed_batch->transactions().end(),
      [this](const auto &tx) {
        if (tx->quorum() < boost::size(tx->signatures())) {
          log_->error(
              "handleOnCompletedBatches: Tx {} required {} signatures, "
              "but got {}",
              tx->toString(),
              tx->quorum(),
              boost::size(tx->signatures()));
        }
      });

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
                      tx->hash(), status_factory_->emptyErrorMessage()));
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

void MstNotificatorImpl::publishPendingStatuses(
    const shared_model::interface::types::SharedTxsCollectionType
        &transactions) {
  publish(transactions, &TxFactoryType::makeMstPending);
}
