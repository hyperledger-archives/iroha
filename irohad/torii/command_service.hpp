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
        : pb_factory_(pb_factory), tx_processor_(txProccesor){};
    /**
     * actual implementation of async Torii in CommandService
     * @param request - Transaction
     * @param response - ToriiResponse
     */
    void ToriiAsync(iroha::protocol::Transaction const& request,
                    iroha::protocol::ToriiResponse& response) {
      auto iroha_tx = pb_factory_.deserialize(request);
      tx_processor_.transaction_handle(iroha_tx);

      tx_processor_.transaction_notifier().subscribe([&response](
          auto iroha_response) {
        auto resp = static_cast<iroha::model::TransactionStatelessResponse&>(
            *iroha_response);
        // iroha-response is shared_ptr of Transaction Response
        // TODO: replace with other responses if needed
        std::cout << "Transaction " << resp.transaction.tx_counter << std::endl;
        if (resp.passed) {
          response.set_validation(
              iroha::protocol::STATELESS_VALIDATION_SUCCESS);
        } else {
          response.set_validation(iroha::protocol::STATELESS_VALIDATION_FAILED);
        }

      });
    }

   private:
    iroha::model::converters::PbTransactionFactory& pb_factory_;
    iroha::torii::TransactionProcessor& tx_processor_;
  };

}  // namespace torii

#endif  // TORII_COMMAND_SERVICE_HPP
