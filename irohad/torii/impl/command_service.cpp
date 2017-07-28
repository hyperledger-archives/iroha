/*
Copyright 2017 Soramitsu Co., Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "torii/command_service.hpp"
#include "common/types.hpp"

namespace torii {

  CommandService::CommandService(
      iroha::model::converters::PbTransactionFactory &pb_factory,
      iroha::torii::TransactionProcessor &txProccesor)
      : pb_factory_(pb_factory), tx_processor_(txProccesor) {
    // Notifier for all clients
    tx_processor_.transactionNotifier().subscribe([this](auto iroha_response) {

      // TODO: make for other responses
      if (iroha:: instanceof
          <iroha::model::TransactionStatelessResponse>(*iroha_response)) {
        auto resp = static_cast<iroha::model::TransactionStatelessResponse &>(
            *iroha_response);
        // Find response in handler map

        auto res =
            this->handler_map_.find(resp.transaction.tx_hash.to_string());

        res->second.set_validation(
            resp.passed ? iroha::protocol::STATELESS_VALIDATION_SUCCESS
                        : iroha::protocol::STATELESS_VALIDATION_FAILED);
      }
    });
  }

  void CommandService::ToriiAsync(iroha::protocol::Transaction const &request,
                                  iroha::protocol::ToriiResponse &response) {
    auto iroha_tx = pb_factory_.deserialize(request);

    handler_map_.insert({iroha_tx->tx_hash.to_string(), response});
    // Send transaction to iroha
    tx_processor_.transactionHandle(iroha_tx);
  }

}  // namespace torii
