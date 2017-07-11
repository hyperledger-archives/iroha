/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#include <network/network_api.h>
#include <consensus/connection/service.hpp>
#include <consensus/consensus_service_stub.hpp>
#include <network/peer_communication_stub.hpp>
#include <ordering/ordering_service_stub.hpp>
#include <torii/processor/stub_query_processor.hpp>
#include <torii/processor/transaction_processor_stub.hpp>
#include <torii/torii_stub.hpp>
#include <validation/chain/validator_stub.hpp>
#include <validation/stateful/stub_validator.hpp>
#include <validation/stateless/validator_impl.hpp>

#include <model/model.hpp>
#include <model/model_crypto_provider_impl.hpp>
#include <crypto/crypto.hpp>
#include <ametsuchi/ametsuchi_stub.hpp>

#include "server_runner.hpp"

int main(int argc, char *argv[]) {

  iroha::model::Transaction transaction;
  transaction.commands.push_back(std::make_shared<iroha::model::AddPeer>());
  torii.get_transaction({}, transaction);

  return 0;
}
