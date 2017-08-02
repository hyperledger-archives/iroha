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

#include "client.hpp"
#include "ametsuchi/block_serializer.hpp"
#include "model/converters/pb_transaction_factory.hpp"

namespace iroha_cli {

  CliClient::CliClient(std::string target_ip, int port)
      : client_(target_ip, port) {}

  CliClient::Status CliClient::sendTx(std::string json_tx) {
    iroha::ametsuchi::BlockSerializer serializer;
    auto tx_opt = serializer.deserialize(json_tx);
    if (not tx_opt.has_value()) {
      return WRONG_FORMAT;
    }
    auto model_tx = tx_opt.value();
    // Convert to protobuf
    iroha::model::converters::PbTransactionFactory factory;
    auto pb_tx = factory.serialize(model_tx);
    // Send to iroha:
    iroha::protocol::ToriiResponse response;
    auto stat = client_.Torii(pb_tx, response);

    return response.validation() ==
                   iroha::protocol::STATELESS_VALIDATION_SUCCESS
               ? OK
               : NOT_VALID;
  }

};  // namespace iroha_cli
