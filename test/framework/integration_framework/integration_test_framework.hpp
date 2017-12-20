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

#ifndef IROHA_INTEGRATION_FRAMEWORK_HPP
#define IROHA_INTEGRATION_FRAMEWORK_HPP

#include <queue>
#include "framework/integration_framework/iroha_instance.hpp"
#include "logger/logger.hpp"
#include "model/block.hpp"
#include "model/proposal.hpp"
#include "model/transaction.hpp"

namespace integration_framework {
  class IntegrationTestFramework {
   public:
    // init
    IntegrationTestFramework &setInitialState(
        const iroha::keypair_t &keypair, const iroha::model::Block &block) {
      log_->info("init state");
      // peer initialization
      iroha_instance_->initPipeline(keypair);
      log_->info("created pipeline");
      iroha_instance_->clearLedger();
      log_->info("cleared ledger");
      iroha_instance_->rawInsertBlock(block);
      log_->info("raw instert");
      // subscribing for components
      iroha_instance_->getIrohaInstance()
          ->getPeerCommunicationService()
          ->on_proposal()
          .subscribe([this](auto proposal) { proposal_queue_.push(proposal); });

      // iroha_instance_->getIrohaInstance()->getPeerCommunicationService()->on_commit().s

      iroha_instance_->run();
      log_->info("run iroha");
      return *this;
    }

    // send tx
    IntegrationTestFramework &sendTx(const iroha::model::Transaction &tx) {
      sendTx(tx, [] {});
      return *this;
    }

    template <typename Lambda>
    IntegrationTestFramework &sendTx(const iroha::model::Transaction &tx,
                                     Lambda validation) {
      log_->info("send transaction");
      // deserialize
      auto pb_tx =
          iroha::model::converters::PbTransactionFactory().serialize(tx);
      // send
      google::protobuf::Empty response;
      iroha_instance_->getIrohaInstance()->getCommandService()->ToriiAsync(
          pb_tx, response);
      // fetch first tx from queue with cv
      // check status
      return *this;
    }

    // proposal
    template <typename Lambda>
    IntegrationTestFramework &checkProposal(Lambda validation) {
      log_->info("check proposal");
      // fetch first proposal from proposal queue

      return *this;
    }

    IntegrationTestFramework &skipProposal() {
      checkProposal([] {});
      return *this;
    }

    // block
    template <typename Lambda>
    IntegrationTestFramework &checkBlock(Lambda validation) {
      // fetch first from block queue
      log_->info("check block");
      return *this;
    }
    IntegrationTestFramework &skipBlock() {
      checkBlock([] {});
      return *this;
    }

    // shutdown iroha
    void done() {
      log_->info("done");
      iroha_instance_->clearLedger();
    }

   protected:
    std::shared_ptr<IrohaInstance> iroha_instance_ =
        std::make_shared<IrohaInstance>();
    std::queue<int> status_queue_;
    std::queue<iroha::model::Proposal> proposal_queue_;
    std::queue<iroha::model::Block> block_queue_;

   private:
    logger::Logger log_ = logger::log("IntegrationTestFramework");
  };
}  // namespace integration_framework

#endif  // IROHA_INTEGRATION_FRAMEWORK_HPP
