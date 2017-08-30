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
      std::shared_ptr<iroha::model::converters::PbTransactionFactory>
          pb_factory,
      std::shared_ptr<iroha::torii::TransactionProcessor> txProccesor)
      : pb_factory_(pb_factory), tx_processor_(txProccesor) {
    // Notifier for all clients
    tx_processor_->transactionNotifier().subscribe([this](auto iroha_response) {

      // Find response in handler map
      auto res = this->handler_map_.find(iroha_response->tx_hash);
      if (res == this->handler_map_.end()){
        iroha::protocol::ToriiResponse response;
        response.set_validation(iroha::protocol::NOT_RECEIVED);
        this->handler_map_.insert({iroha_response->tx_hash, response});
        return;
      }
      this->handler_map_.insert({iroha_response->tx_hash, res->second});
    });
  }

  void CommandService::ToriiAsync(iroha::protocol::Transaction const &request,
                                  google::protobuf::Empty &empty) {
    auto iroha_tx = pb_factory_->deserialize(request);

    auto tx_hash = iroha_tx->tx_hash.to_string();

    iroha::protocol::ToriiResponse response;
    response.set_validation(iroha::protocol::StatelessValidation::ON_PROCESS);

    if (handler_map_.count(tx_hash) > 0) {
      return;
    }

    handler_map_.emplace(tx_hash, response);
    // Send transaction to iroha
    tx_processor_->transactionHandle(iroha_tx);
  }

  void CommandService::StatusAsync(
      iroha::protocol::TxStatusRequest const &request,
      iroha::protocol::ToriiResponse &response) {
    auto resp = handler_map_.find(request.tx_hash());

    if (resp == handler_map_.end()) {
      response.set_validation(
          iroha::protocol::StatelessValidation::NOT_RECEIVED);
      return;
    }
    response.CopyFrom(resp->second);
  }

}  // namespace torii
