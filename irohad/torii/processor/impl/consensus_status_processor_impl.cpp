/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "torii/processor/consensus_status_processor_impl.hpp"

#include <boost/format.hpp>

#include "interfaces/iroha_internal/block.hpp"
#include "interfaces/iroha_internal/proposal.hpp"
#include "interfaces/iroha_internal/transaction_sequence.hpp"
#include "validation/stateful_validator_common.hpp"

namespace iroha {
  namespace torii {

    using network::PeerCommunicationService;

    namespace {
      std::string composeErrorMessage(
          const validation::TransactionError &tx_error) {
        if (not tx_error.first.tx_passed_initial_validation) {
          return (boost::format("Stateful validation error: transaction %s "
                                "did not pass initial verification: "
                                "checking '%s', error message '%s'")
                  % tx_error.second.hex() % tx_error.first.name
                  % tx_error.first.error)
              .str();
        }
        return (boost::format("Stateful validation error in transaction %s: "
                              "command '%s' with index '%d' did not pass "
                              "verification with error '%s'")
                % tx_error.second.hex() % tx_error.first.name
                % tx_error.first.index % tx_error.first.error)
            .str();
      }
    }  // namespace

    ConsensusStatusProcessorImpl::ConsensusStatusProcessorImpl(
        std::shared_ptr<PeerCommunicationService> pcs,
        std::shared_ptr<iroha::torii::StatusBus> status_bus,
        std::shared_ptr<shared_model::interface::TxStatusFactory>
            status_factory)
        : pcs_(std::move(pcs)),
          status_bus_(std::move(status_bus)),
          status_factory_(std::move(status_factory)),
          log_(logger::log("TxProcessor")) {
      // process stateful validation results
      addSubscription(pcs_->on_verified_proposal().subscribe(
          [this](std::shared_ptr<validation::VerifiedProposalAndErrors>
                     proposal_and_errors) {
            handleOnVerifiedProposal(proposal_and_errors);
          }));

      // commit transactions
      addSubscription(pcs_->on_commit().subscribe(
          [this](synchronizer::SynchronizationEvent sync_event) {
            handleOnCommit(sync_event);
          }));
    }

    void ConsensusStatusProcessorImpl::publishStatus(
        shared_model::interface::TxStatusFactory::TxStatusFactoryInvoker
            invoker,
        const shared_model::crypto::Hash &hash,
        const std::string &error) const {
      status_bus_->publish((status_factory_.get()->*invoker)(hash, error));
    }

    void ConsensusStatusProcessorImpl::handleOnVerifiedProposal(
        std::shared_ptr<iroha::validation::VerifiedProposalAndErrors>
            proposal_and_errors) {
      // notify about failed txes
      const auto &errors = proposal_and_errors->second;
      std::lock_guard<std::mutex> lock(notifier_mutex_);
      for (const auto &tx_error : errors) {
        auto error_msg = composeErrorMessage(tx_error);
        log_->info(error_msg);
        this->publishStatus(
            &TxFatoryType::makeStatefulFail, tx_error.second, error_msg);
      }
      // notify about success txs
      for (const auto &successful_tx :
           proposal_and_errors->first->transactions()) {
        log_->info("on stateful validation success: {}",
                   successful_tx.hash().hex());
        this->publishStatus(&TxFatoryType::makeStatefulValid,
                            successful_tx.hash(),
                            TxFatoryType::emptyErrorMassage());
      }
    }

    void ConsensusStatusProcessorImpl::handleOnCommit(
        const iroha::synchronizer::SynchronizationEvent &sync_event) {
      sync_event.synced_blocks.subscribe(
          // on next
          [this](auto model_block) {
            current_txs_hashes_.reserve(model_block->transactions().size());
            std::transform(model_block->transactions().begin(),
                           model_block->transactions().end(),
                           std::back_inserter(current_txs_hashes_),
                           [](const auto &tx) { return tx.hash(); });
          },
          // on complete
          [this] {
            if (current_txs_hashes_.empty()) {
              log_->info("there are no transactions to be committed");
            } else {
              std::lock_guard<std::mutex> lock(notifier_mutex_);
              for (const auto &tx_hash : current_txs_hashes_) {
                log_->info("on commit committed: {}", tx_hash.hex());
                this->publishStatus(&TxFatoryType::makeCommitted,
                                    tx_hash,
                                    TxFatoryType::emptyErrorMassage());
              }
              current_txs_hashes_.clear();
            }
          });
    }
  }  // namespace torii
}  // namespace iroha
