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
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include "logger/logger.hpp"
#include "ametsuchi/impl/storage_impl.hpp"
#include "genesis_block_client.hpp"
#include "genesis_block_service.hpp"
#include "validators.hpp"
#include "model/transaction.hpp"
#include "model/block.hpp"
#include "ametsuchi/block_serializer.hpp"

// ** Genesis Block and Provisioning ** //

void create_account(std::string name);

// Reference is here (TODO: move to doc):
// https://hackmd.io/GwRmwQ2BmCFoCsAGARtOAWBIBMcAcS0GcAZjhNNPvpAKZIDGQA==

logger::Logger Log("iroha-cli");

DEFINE_string(config, "", "Trusted peers' ip");
DEFINE_validator(config, &iroha_cli::validate_config);

DEFINE_string(genesis_block, "", "Genesis block for sending network");
DEFINE_validator(genesis_block, &iroha_cli::validate_genesis_block);

DEFINE_bool(new_account, false, "Choose if account does not exist");
DEFINE_string(name, "", "Name of the account");

void fatal_error(std::string const& error) {
  Log.error(error);
  exit(-1);
}

void assert_fatal(bool condition, std::string const& error) {
  if (!condition) {
    fatal_error(error);
  }
}

std::vector<std::string> parse_config_trusted_peers(std::ifstream& ifs) {
  std::vector<std::string> ret;
  rapidjson::Document doc;
  rapidjson::IStreamWrapper isw(ifs);
  doc.ParseStream(isw);
  assert_fatal(doc.HasParseError(), "JSON Parse error: " + FLAGS_config);

  const char* MemberIp = "ip";
  assert_fatal(doc.HasMember(MemberIp), "In '" + FLAGS_config +
    "', member '" + std::string(MemberIp) + "' doesn't exist.");
  for (const auto& ip : doc["ip"].GetArray()) {
    assert_fatal(ip.IsString(), "'" + std::string(MemberIp) + "' has not string value.");
    ret.push_back(ip.GetString());
  }
  return ret;
}

iroha::model::Block parse_genesis_block(std::ifstream &ifs) {
  const char* MemberTxs = "transactions";

  rapidjson::Document doc;
  rapidjson::IStreamWrapper isw(ifs);
  doc.ParseStream(isw);

  assert_fatal(doc.HasParseError(), FLAGS_config + ": parse error");
  assert_fatal(doc.IsObject(),      "JSON is not object.");
  assert_fatal(doc.HasMember(MemberTxs), "No member '" + std::string(MemberTxs) + "'");
  assert_fatal(doc[MemberTxs].IsArray(), std::string(MemberTxs) + " is not array.");

  iroha::model::Block block;
  auto block_serializer = iroha::ametsuchi::BlockSerializer();
  auto txs = block_serializer.deserialize_transactions(doc);
  assert_fatal(txs.has_value(), "Failed to deserialize transaction");
  block.transactions = *txs;
  // TODO: Add more members to block.
  return block;
}

void abort_network(std::vector<std::string> const& trusted_peers, iroha::model::Block &block) {
  for (const auto& ip : trusted_peers) {
    iroha_cli::GenesisBlockClient client(ip, iroha::GenesisBlockServicePort);
    client.SendAbortGenesisBlock(block);
  }
}

void bootstrap_network() {
  /**
   * parse transactions from `genesis.json` (FLAGS_genesis_block)
   * and then send block to trusted peers `target.json` (FLAGS_config)
   * where each iroahd already wakes up.
   */
  std::ifstream ifs_config(FLAGS_config);
  assert_fatal(ifs_config.is_open(), "Cannot open: '" + FLAGS_config + "'");
  auto trusted_peers = parse_config_trusted_peers(ifs_config);

  std::ifstream ifs_genesis(FLAGS_genesis_block);
  assert_fatal(ifs_genesis.is_open(), "Cannot open: '" + FLAGS_genesis_block + "'");
  auto genesis_block = parse_genesis_block(ifs_genesis);

  // send block to trusted peers.
  for (const auto& ip : trusted_peers) {
    iroha_cli::GenesisBlockClient client(ip, iroha::GenesisBlockServicePort);
    iroha::protocol::ApplyGenesisBlockResponse response;
    auto stat = client.SendGenesisBlock(genesis_block, response);
    if (!stat.ok() || response.applied() == iroha::protocol::APPLY_FAILURE) {
      abort_network(trusted_peers, genesis_block);
      assert_fatal(false, "Failure of creating genesis block in Ip: '" + ip + "'");
    }
  }
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  if (FLAGS_new_account) {
    // Create new pub/priv key
    if (std::ifstream(FLAGS_name + ".pub")) {
      assert_fatal(false, "File already exists");
    }
    create_account(FLAGS_name);
  }
  else if (!FLAGS_config.empty() && !FLAGS_genesis_block.empty()) {
    bootstrap_network();
  }
  else {
    assert_fatal(false, "Invalid flags");
  }

  /*
  else if (FLAGS_grpc) {
    // Send test tx to Iroha
    if (FLAGS_port > 0 && FLAGS_port < 65535) {
      std::cout<< "Send transaction to " << FLAGS_address << ":" << FLAGS_port << std::endl;
      iroha::protocol::Transaction request;
      iroha::protocol::ToriiResponse response;
      torii::CommandSyncClient(FLAGS_address, FLAGS_port)
        .Torii(request, response);
    } else {
      std::cout << "Invalid port number " << FLAGS_port << std::endl;
      //iroha_cli::CliClient(FLAGS_address, FLAGS_port);
    }
  }
  */

  /*
  if (FLAGS_new_ledger) {
    auto storage = iroha::ametsuchi::StorageImpl::create(
        FLAGS_path, FLAGS_redis, FLAGS_redis_port, FLAGS_pg_conn);
    auto mut = storage->createMutableStorage();
     auto block;
     storage.apply(block, [](const auto& current_block, auto& executor,
                             auto& query, auto& top_block) {
       for (const auto& tx : current_block.transactions) {
         for (const auto& command : tx.commands) {
           if (not command->execute(query, executor)) {
             return false;
           }
         }
       }
       return true;
     });
  }
*/
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
