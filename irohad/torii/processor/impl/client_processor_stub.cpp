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

#include <torii/processor/client_processor_stub.hpp>

namespace iroha {
  namespace torii {
    ClientProcessorStub::ClientProcessorStub(
        const validation::StatelessValidator &validator,
        network::PeerCommunicationService &service,
        dao::DaoCryptoProvider &provider)
        : QueryProcessorStub(),
          TransactionProcessorStub(validator, service, provider) {}

    void ClientProcessorStub::query_handle(dao::Client client,
                                           dao::Query &query) {
      QueryProcessorStub::query_handle(client, query);
    }

    rxcpp::observable<std::shared_ptr<dao::QueryResponse>>
    ClientProcessorStub::query_notifier() {
      return QueryProcessorStub::query_notifier();
    }

    void ClientProcessorStub::transaction_handle(
        dao::Client client, dao::Transaction &transaction) {
      TransactionProcessorStub::transaction_handle(client, transaction);
    }

    rxcpp::observable<dao::TransactionResponse>
    ClientProcessorStub::transaction_notifier() {
      return TransactionProcessorStub::transaction_notifier();
    }
  }  // namespace torii
}