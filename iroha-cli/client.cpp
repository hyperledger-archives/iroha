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

#include "crypto/crypto.hpp"
#include "common/types.hpp"

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
    // Get private and public key of transaction creator
    std::string client_pub_key_;
    std::string client_priv_key_;
    std::ifstream priv_file(model_tx.creator_account_id + ".priv");
    if (not priv_file) {
      return NO_ACCOUNT;
    }
    priv_file >> client_priv_key_;
    priv_file.close();

    std::ifstream pub_file(model_tx.creator_account_id + ".pub");
    if (not pub_file) {
      return NO_ACCOUNT;
    }
    pub_file >> client_pub_key_;
    pub_file.close();

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
    // TODO: Fix signature to work with data, not hash
    sign.signature =
        iroha::sign(tx_hash.data(), tx_hash.size(), ed_pubkey, ed_privkey);
    model_tx.signatures = {sign};
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

  void CliClient::create_account(std::string account_name, std::string pass_phrase) {

    auto seed = iroha::create_seed(pass_phrase);
    auto key_pairs = iroha::create_keypair(seed);

    // Save pubkey to file
    std::ofstream pub_file(account_name + ".pub");
    pub_file << key_pairs.pubkey.to_hexstring();
    // Save privkey to file
    std::ofstream priv_file(account_name + ".priv");
    priv_file << key_pairs.privkey.to_hexstring();
  }

};  // namespace iroha_cli
