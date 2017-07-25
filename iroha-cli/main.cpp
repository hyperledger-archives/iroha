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

#include <ametsuchi/impl/storage_impl.hpp>
#include "client.hpp"
#include "validators.hpp"

DEFINE_bool(new_account, false, "Choose if account does not exist");
DEFINE_string(name, "", "Name of the account");

DEFINE_bool(grpc, false, "Send sample transaction to IrohaNetwork");
DEFINE_string(address, "127.0.0.1", "Address of the Iroha node");
// todo: host validator
DEFINE_int32(torii_port, 50051, "Port of iroha's Torii");
DEFINE_validator(torii_port, &iroha_cli::validate_port);

DEFINE_bool(new_ledger, false,
            "Creates new database and genesis block for the ledger");
DEFINE_string(path, "/var/iroha/db/", "Path of the Ametsuchi");
// todo: path validator
DEFINE_string(redis, "127.0.0.1", "Redis address");
DEFINE_int32(redis_port, 6379, "Redis port");
DEFINE_validator(redis_port, &iroha_cli::validate_port);
DEFINE_string(pg_conn, "host=localhost",
              "Postgres parameters,"
              "check out http://tinyurl.com/ycspggge");
DEFINE_string(peers, "", "Public keys of iroha nodes separated by \";\"");
DEFINE_validator(peers, &iroha_cli::validate_peers);

void create_account(std::string name);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  gflags::ShutDownCommandLineFlags();

  if (FLAGS_new_account) {
    if (std::ifstream(FLAGS_name + ".pub")) {
      std::cout << "File already exists" << std::endl;
      return -1;
    }
    create_account(FLAGS_name);

    // Send test tx to Iroha
  }

  if (FLAGS_grpc) {
    std::cout << "Send transaction to " << FLAGS_address << ":"
              << FLAGS_torii_port << std::endl;
    auto client = iroha_cli::CliClient(FLAGS_address, FLAGS_torii_port);
    // ToDo more variables transaction
    client.sendTx(iroha::model::Transaction{});
    return 0;
  }

  if (FLAGS_new_ledger) {
    auto storage = iroha::ametsuchi::StorageImpl::create(
        FLAGS_path, FLAGS_redis, FLAGS_redis_port, FLAGS_pg_conn);
    auto mut = storage->createMutableStorage();
    // auto block;
    // storage.apply(block, [](const auto& current_block, auto& executor,
    //                         auto& query, auto& top_block) {
    //   for (const auto& tx : current_block.transactions) {
    //     for (const auto& command : tx.commands) {
    //       if (not command->execute(query, executor)) {
    //         return false;
    //       }
    //     }
    //   }
    //   return true;
    // });
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
