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

#ifndef IROHA_TRANSACTION_PROCESSOR_STUB_HPP
#define IROHA_TRANSACTION_PROCESSOR_STUB_HPP

#include <network/network_api.h>
#include <model/model_crypto_provider.hpp>
#include <torii/processor/transaction_processor.hpp>
#include <validation/stateless/validator.hpp>
#include <ordering/ordering_service.hpp>

namespace iroha {
  namespace torii {
    class TransactionProcessorStub : public TransactionProcessor {
     public:

      /**
       * @param pcs - provide information proposals and commits
       * @param os - ordering service for sharing transactions
       * @param validator - perform stateless validation
       * @param crypto_provider - sign income transactions
       */
      TransactionProcessorStub(network::PeerCommunicationService &pcs,
                               ordering::OrderingService &os,
                               const validation::StatelessValidator &validator,
                               model::ModelCryptoProvider &crypto_provider);

      void transaction_handle(model::Client client,
                              model::Transaction &transaction) override;

      rxcpp::observable<std::shared_ptr<model::TransactionResponse>>
      transaction_notifier() override;

     private:
      // connections
      network::PeerCommunicationService &pcs_;
      ordering::OrderingService &os_;

      // processing
      const validation::StatelessValidator &validator_;
      model::ModelCryptoProvider &crypto_provider_;

      // internal
      rxcpp::subjects::subject<std::shared_ptr<model::TransactionResponse>>
          notifier_;
    };
  }  // namespace torii
}  // namespace iroha

#endif  // IROHA_TRANSACTION_PROCESSOR_STUB_HPP
