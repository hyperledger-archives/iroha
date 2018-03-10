/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "torii/processor/transaction_processor_impl.hpp"
#include <iostream>
#include <utility>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "backend/protobuf/from_old_model.hpp"
#include "backend/protobuf/transaction.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "interfaces/iroha_internal/proposal.hpp"
#include "model/sha3_hash.hpp"
#include "model/transaction_response.hpp"

namespace iroha {
  namespace torii {

    using model::TransactionResponse;
    using Status = model::TransactionResponse::Status;
    using network::PeerCommunicationService;

    TransactionProcessorImpl::TransactionProcessorImpl(
        std::shared_ptr<PeerCommunicationService> pcs,
        std::shared_ptr<MstProcessor> mst_processor)
        : pcs_(std::move(pcs)), mst_processor_(std::move(mst_processor)) {
      log_ = logger::log("TxProcessor");

      // insert all txs from proposal to proposal set
      pcs_->on_proposal().subscribe([this](auto model_proposal) {
        auto proposal =
            std::unique_ptr<model::Proposal>(model_proposal->makeOldModel());
        for (const auto &tx : proposal->transactions) {
          proposal_set_.insert(hash(tx).to_string());
          this->notify(hash(tx).to_string(),
                       Status::STATELESS_VALIDATION_SUCCESS);
        }
      });

      // move commited txs from proposal to candidate map
      pcs_->on_commit().subscribe([this](Commit blocks) {
        blocks.subscribe(
            // on next..
            [this](auto &&block) {
              const auto in_proposal = [this](const auto &tx) {
                return this->proposal_set_.count(
                    shared_model::crypto::toBinaryString(
                        static_cast<const shared_model::proto::Transaction *>(
                            tx.operator->())
                            ->hash()));
              };
              boost::for_each(
                  block->transactions()
                      | boost::adaptors::filtered(in_proposal),
                  [this](auto &&t) {
                    const auto &h = shared_model::crypto::toBinaryString(
                        static_cast<const shared_model::proto::Transaction *>(
                            t.operator->())
                            ->hash());
                    this->proposal_set_.erase(h);
                    this->candidate_set_.insert(h);
                    this->notify(h, Status::STATEFUL_VALIDATION_SUCCESS);
                  });
            },
            // on complete
            [this]() {
              boost::for_each(this->proposal_set_, [this](auto &&t) {
                return this->notify(t, Status::STATEFUL_VALIDATION_FAILED);
              });
              this->proposal_set_.clear();
              boost::for_each(this->candidate_set_, [this](auto &&t) {
                return this->notify(t, Status::COMMITTED);
              });
              this->candidate_set_.clear();
            });
      });

      mst_processor_->onPreparedTransactions().subscribe([this](auto &&tx) {
        log_->info("MST tx prepared");
        return this->transactionHandle(
            std::shared_ptr<iroha::model::Transaction>(tx->makeOldModel()));
      });
      mst_processor_->onExpiredTransactions().subscribe([this](auto &&tx) {
        log_->info("MST tx expired");
        return this->notify(
            shared_model::crypto::toBinaryString(
                static_cast<shared_model::proto::Transaction &>(*tx).hash()),
            Status::MST_EXPIRED);
      });
    }

    void TransactionProcessorImpl::transactionHandle(
        std::shared_ptr<model::Transaction> transaction) {
      log_->info("handle transaction");
      const auto &h = hash(*transaction).to_string();

      if (transaction->signatures.size() < transaction->quorum) {
        log_->info("waiting for quorum signatures");
        mst_processor_->propagateTransaction(
            std::make_shared<shared_model::proto::Transaction>(
                shared_model::proto::from_old(*transaction)));
      } else {
        log_->info("propagating tx");
        pcs_->propagate_transaction(
            std::make_shared<shared_model::proto::Transaction>(
                shared_model::proto::from_old(*transaction)));
      }

      log_->info("stateless validated");
      notify(h, Status::STATELESS_VALIDATION_SUCCESS);
    }

    rxcpp::observable<std::shared_ptr<model::TransactionResponse>>
    TransactionProcessorImpl::transactionNotifier() {
      return notifier_.get_observable();
    }

    void TransactionProcessorImpl::notify(const std::string &hash, Status s) {
      notifier_.get_subscriber().on_next(
          std::make_shared<TransactionResponse>(hash, s));
    }
  }  // namespace torii
}  // namespace iroha
