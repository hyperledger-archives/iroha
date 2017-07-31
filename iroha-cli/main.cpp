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

#include <ed25519.h>
#include <gflags/gflags.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "validators.hpp"
#include "bootstrap_network.hpp"
#include "common/assert_config.hpp"
#include "genesis_block_client_impl.hpp"

#include "client.hpp"

// ** Genesis Block and Provisioning ** //
// Reference is here (TODO: move to doc):
// https://hackmd.io/GwRmwQ2BmCFoCsAGARtOAWBIBMcAcS0GcAZjhNNPvpAKZIDGQA==

DEFINE_string(config, "", "Trusted peer's ip addresses");
//DEFINE_validator(config, &iroha_cli::validate_config);

DEFINE_string(genesis_block, "", "Genesis block for sending network");
//DEFINE_validator(genesis_block, &iroha_cli::validate_genesis_block);

DEFINE_bool(new_account, false, "Choose if account does not exist");
DEFINE_string(name, "", "Name of the account");

// Sending transaction to Iroha
DEFINE_bool(grpc, false, "Send sample transaction to IrohaNetwork");
DEFINE_string(address, "127.0.0.1", "Address of the Iroha node");
DEFINE_int32(torii_port, 50051, "Port of iroha's Torii");
//DEFINE_validator(torii_port, &iroha_cli::validate_port);
DEFINE_string(json_transaction, "", "Transaction in json format");


void create_account(std::string name);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  if (FLAGS_new_account) {
    // Create new pub/priv key
    if (std::ifstream(FLAGS_name + ".pub")) {
      assert_config::assert_fatal(false, "File already exists");
    }
    create_account(FLAGS_name);
  } else if (not FLAGS_config.empty() && not FLAGS_genesis_block.empty()) {
    iroha_cli::GenesisBlockClientImpl genesis_block_client;
    auto bootstrap = iroha_cli::BootstrapNetwork(genesis_block_client);
    auto peers = bootstrap.parse_trusted_peers(FLAGS_config);
    auto block = bootstrap.parse_genesis_block(FLAGS_genesis_block);
    block = bootstrap.merge_tx_add_trusted_peers(block, peers);
    bootstrap.run_network(peers, block);
  } else  if (FLAGS_grpc) {
    std::cout << "Send transaction to " << FLAGS_address << ":"
              << FLAGS_torii_port << std::endl;

    iroha_cli::CliClient client(FLAGS_address, FLAGS_torii_port, FLAGS_name);

    client.sendTx(FLAGS_json_transaction);
    return 0;
  } else {
    assert_config::assert_fatal(false, "Invalid flags");
  }
  return 0;
}

std::string hex_str(unsigned char* data, int len) {
  constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                             '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string s((unsigned long)(len * 2), ' ');
  for (int i = 0; i < len; ++i) {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

/**
 * Command to create a new account using the interactive console.
 */
void create_account(std::string name) {
  unsigned char public_key[32], private_key[64], seed[32];

  ed25519_create_keypair(public_key, private_key, seed);
  auto pub_hex = hex_str(public_key, 32);

  auto priv_hex = hex_str(private_key, 64);

  // Save pubkey to file
  std::ofstream pub_file(name + ".pub");
  pub_file << pub_hex;
  pub_file.close();

  // Save privkey to file
  std::ofstream priv_file(name + ".priv");
  priv_file << priv_hex;
  priv_file.close();

  std::cout << "Public and private key has been generated in current directory"
            << std::endl;
}
