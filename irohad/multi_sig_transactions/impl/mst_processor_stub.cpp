/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "multi_sig_transactions/mst_processor_stub.hpp"

#include "interfaces/iroha_internal/transaction_batch.hpp"
#include "interfaces/transaction.hpp"

using namespace iroha;

auto MstProcessorStub::propagateBatchImpl(const DataType &batch)
    -> decltype(propagateBatch(batch)) {
  if (std::any_of(batch->transactions().begin(),
                  batch->transactions().end(),
                  [](const auto &tx) { return tx->quorum() > 1; })) {
    log_->warn(
        "Multisig transactions are disabled. Anyway, Iroha is going to "
        "propagate batch: {}",
        batch->reducedHash().toString());
  }
  prepared_subject_.get_subscriber().on_next(batch);
}

auto MstProcessorStub::onStateUpdateImpl() const -> decltype(onStateUpdate()) {
  log_->warn(
      "Multisig transactions are disabled, so MstProcessor observable won't "
      "emit any events");
  return rxcpp::observable<>::empty<std::shared_ptr<MstState>>();
}

auto MstProcessorStub::onPreparedBatchesImpl() const
    -> decltype(onPreparedBatches()) {
  return prepared_subject_.get_observable();
}

auto MstProcessorStub::onExpiredBatchesImpl() const
    -> decltype(onExpiredBatches()) {
  log_->warn(
      "Multisig transactions are disabled, so MstProcessor observable won't "
      "emit any events");
  return rxcpp::observable<>::empty<DataType>();
}
