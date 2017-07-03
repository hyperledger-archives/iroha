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

#ifndef IROHA_CLIENT_PROCESSOR_STUB_HPP
#define IROHA_CLIENT_PROCESSOR_STUB_HPP

#include <torii/processor/client_processor.hpp>
#include <torii/processor/stub_query_processor.hpp>
#include <torii/processor/transaction_processor_stub.hpp>

namespace iroha {
  namespace torii {

    /**
     * Client processor is interface
     * for processing all user's intents in the system
     */
    class ClientProcessorStub : public ClientProcessor,
                                public QueryProcessorStub,
                                public TransactionProcessorStub {
     public:
      ClientProcessorStub(const validation::StatelessValidator &validator,
                          network::PeerCommunicationService &service,
                          dao::DaoCryptoProvider &provider);
      void query_handle(dao::Client client, const dao::Query &query) override;
      rxcpp::observable<std::shared_ptr<dao::QueryResponse>> query_notifier() override;
      void transaction_handle(dao::Client client, dao::Transaction &transaction) override;
      rxcpp::observable<dao::TransactionResponse> transaction_notifier() override;
    };
  }  // namespace torii
}

#endif  // IROHA_CLIENT_PROCESSOR_STUB_HPP
