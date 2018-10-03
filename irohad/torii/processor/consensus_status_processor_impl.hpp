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

#ifndef IROHA_CONSENSUS_STATUS_PROCESSOR_IMPL_HPP
#define IROHA_CONSENSUS_STATUS_PROCESSOR_IMPL_HPP

#include <mutex>

#include <rxcpp/rx.hpp>

#include "builders/default_builders.hpp"
#include "interfaces/iroha_internal/tx_status_factory.hpp"
#include "interfaces/transaction_responses/tx_response.hpp"
#include "logger/logger.hpp"
#include "multi_sig_transactions/mst_processor.hpp"
#include "network/peer_communication_service.hpp"
#include "torii/processor/consensus_status_processor.hpp"
#include "torii/status_bus.hpp"

namespace iroha {
  namespace torii {
    class ConsensusStatusProcessorImpl : public ConsensusStatusProcessor {
     public:
      /**
       * @param pcs - provide information proposals and commits
       * @param mst_processor is a handler for multisignature transactions
       * @param status_bus is a common notifier for tx statuses
       * @param status_factory is factory for status creation
       */
      ConsensusStatusProcessorImpl(
          std::shared_ptr<network::PeerCommunicationService> pcs,
          std::shared_ptr<iroha::torii::StatusBus> status_bus,
          std::shared_ptr<shared_model::interface::TxStatusFactory>
              status_factory);

      void handleOnVerifiedProposal(
          std::shared_ptr<validation::VerifiedProposalAndErrors>
              validation_outcome) override;

      void handleOnCommit(
          const iroha::synchronizer::SynchronizationEvent &) override;

     private:
      // connections
      std::shared_ptr<network::PeerCommunicationService> pcs_;

      std::shared_ptr<iroha::torii::StatusBus> status_bus_;

      // internal
      rxcpp::subjects::subject<
          std::shared_ptr<shared_model::interface::TransactionResponse>>
          notifier_;

      // keeps hashes of transaction, which were committed during this round
      std::vector<shared_model::interface::types::HashType> current_txs_hashes_;

      std::shared_ptr<shared_model::interface::TxStatusFactory> status_factory_;

      logger::Logger log_;

      /// prevents from emitting new tx statuses from different threads
      /// in parallel
      std::mutex notifier_mutex_;

      // shortcut for the type
      using TxFatoryType = shared_model::interface::TxStatusFactory;

      /**
       * Publish status of transaction
       * @param invoker is function which creates appropriate status
       * @param hash of that transaction
       * @param error, which can appear during validation
       */
      void publishStatus(TxFatoryType::TxStatusFactoryInvoker invoker,
                         const shared_model::crypto::Hash &hash,
                         const std::string &error = "") const;
    };
  }  // namespace torii
}  // namespace iroha

#endif  // IROHA_CONSENSUS_STATUS_PROCESSOR_IMPL_HPP
