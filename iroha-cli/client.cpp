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

// In iroha-cli only, " is used.
#include "client.hpp"

#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc/grpc.h>

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>

#include "ametsuchi/block_serializer.hpp"
#include "model/converters/pb_transaction_factory.hpp"
#include "model/model_hash_provider_impl.hpp"

namespace iroha_cli {

  CliClient::CliClient(std::string target_ip, int port, std::string name)
      : client_(target_ip, port) {
    std::ifstream priv_file(name + ".priv");
    priv_file >> client_priv_key_;
    priv_file.close();

    std::ifstream pub_file(name + ".pub");
    pub_file >> client_pub_key_;
    pub_file.close();
  }

  std::string CliClient::sendTx(std::string json_tx) {
    iroha::ametsuchi::BlockSerializer serializer;
    auto tx_opt = serializer.deserialize(json_tx);
    if (not tx_opt.has_value()) {
      return "Wrong transaction format";
    }

    auto model_tx = tx_opt.value();
    // Get hash of transaction
    iroha::model::HashProviderImpl hashProvider;
    auto tx_hash = hashProvider.get_hash(model_tx);
    auto pubkey = iroha::hex2bytes(client_pub_key_);
    auto privkey = iroha::hex2bytes(client_priv_key_);

    iroha::ed25519::pubkey_t ed_pubkey;
    std::copy(pubkey.begin(), pubkey.end(), ed_pubkey.begin());

    iroha::ed25519::privkey_t ed_privkey;
    std::copy(privkey.begin(), privkey.end(), ed_privkey.begin());
    // Sign transaction
    iroha::model::Signature sign;
    sign.pubkey = ed_pubkey;
    sign.signature =
        iroha::sign(tx_hash.data(), tx_hash.size(), ed_pubkey, ed_privkey);
    model_tx.signatures = {sign};
    // Convert to protobuf
    iroha::model::converters::PbTransactionFactory factory;
    auto pb_tx = factory.serialize(model_tx);
    // Send to iroha:
    iroha::protocol::ToriiResponse response;
    auto stat = client_.Torii(pb_tx, response);
    if (response.validation() ==
        iroha::protocol::STATELESS_VALIDATION_SUCCESS) {
      return "Stateless validation success";
    } else {
      return "Stateless validation error";
    }
  }

};  // namespace iroha_cli
