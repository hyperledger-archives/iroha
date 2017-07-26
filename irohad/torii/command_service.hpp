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

#ifndef TORII_COMMAND_SERVICE_HPP
#define TORII_COMMAND_SERVICE_HPP

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>
#include <iostream>
#include "model/converters/pb_transaction_factory.hpp"
#include "model/tx_responses/stateless_response.hpp"
#include "torii/processor/transaction_processor.hpp"
#include <unordered_map>

namespace torii {

  /**
   * Actual implementation of async CommandService.
   * ToriiServiceHandler::(SomeMethod)Handler calls a corresponding method in
   * this class.
   */
  class CommandService {
   public:
    CommandService(iroha::model::converters::PbTransactionFactory& pb_factory,
                   iroha::torii::TransactionProcessor& txProccesor)
        : pb_factory_(pb_factory), tx_processor_(txProccesor) {

      tx_processor_.transaction_notifier().subscribe([this](
                                                         auto iroha_response) {

        std::cout << "On_next event trigered " << std::endl;
        // TODO : dynamic cast
        auto resp = static_cast<iroha::model::TransactionStatelessResponse&>(
            *iroha_response);

        std::cout << "Received response for transaction "
                  << resp.transaction.tx_counter << std::endl;

        if (resp.passed) {
          std::cout << "Transaction is valid " << std::endl;

          auto res = this->handler_map_.find(resp.transaction.tx_counter);

          if (res != this->handler_map_.end()){
            std::cout << "Handler found " << std::endl;
            res->second.set_validation(iroha::protocol::STATELESS_VALIDATION_SUCCESS);
          }
          else{
            std::cout << "No handler found " << std::endl;
          }
        }
      });
    };
    /**
     * actual implementation of async Torii in CommandService
     * @param request - Transaction
     * @param response - ToriiResponse
     */
    void ToriiAsync(iroha::protocol::Transaction const& request,
                    iroha::protocol::ToriiResponse& response) {
      auto iroha_tx = pb_factory_.deserialize(request);
      std::cout << "Setting handler for tx " << iroha_tx.tx_counter << std::endl;
      handler_map_.insert({iroha_tx.tx_counter, response});
      tx_processor_.transaction_handle(iroha_tx);


    }

   private:
    iroha::model::converters::PbTransactionFactory& pb_factory_;
    iroha::torii::TransactionProcessor& tx_processor_;
    std::unordered_map<uint64_t, iroha::protocol::ToriiResponse&> handler_map_;
  };

}  // namespace torii

#endif  // TORII_COMMAND_SERVICE_HPP
