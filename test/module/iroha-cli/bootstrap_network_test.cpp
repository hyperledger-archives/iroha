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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "bootstrap_network.hpp"
#include <grpc++/grpc++.h>
#include <fstream>
#include <memory>
#include "genesis_block_client.hpp"
#include "model/converters/pb_command_factory.hpp"
#include "common/types.hpp"
#include "crypto/crypto.hpp"
#include "endpoint.grpc.pb.h"
#include "main/genesis_block_server/genesis_block_server.hpp"
#include "model/block.hpp"
#include "model/model_hash_provider_impl.hpp"

using ::testing::Return;

class iroha_cli_test : public ::testing::Test {
 public:
  static iroha::model::Block create_genesis_block() {
    iroha::model::Transaction tx;
    tx.created_ts = 111111;
    tx.tx_counter = 987654;
    tx.creator_account_id = "user1";
    iroha::model::CreateDomain createDomain;
    createDomain.domain_name = "ja";
    tx.commands.push_back(
        std::make_shared<iroha::model::CreateDomain>(createDomain));

    iroha::model::Block block;
    block.transactions.push_back(tx);
    block.height = 1;
    block.prev_hash.fill(0);
    block.merkle_root.fill(0);
    block.hash.fill(0);
    block.created_ts = 12345678;
    block.txs_number = block.transactions.size();
    iroha::model::HashProviderImpl hash_provider;
    block.hash = hash_provider.get_hash(block);
    return block;
  }

  std::string rnd_hex_pk() {
    return iroha::create_keypair(iroha::create_seed()).pubkey.to_hexstring();
  }
};

class MockGenesisBlockClient : public iroha_cli::GenesisBlockClient {
  /*
  MOCK_METHOD2(set_channel, void(const std::string &ip, const int port));
  MOCK_METHOD2(
      send_genesis_block,
      grpc::Status(const iroha::model::Block &iroha_block,
                   iroha::protocol::ApplyGenesisBlockResponse &response));
  MOCK_METHOD1(send_abort_genesis_block,
               void(const iroha::model::Block &block));
               */

  void set_channel(const std::string &ip, const int port) {
    // DO NOTHING
  }

  grpc::Status send_genesis_block(
      const iroha::model::Block &iroha_block,
      iroha::protocol::ApplyGenesisBlockResponse &response) {
    auto expected_block = iroha_cli_test::create_genesis_block();
    response.set_applied(iroha_block == expected_block
                             ? iroha::protocol::APPLY_SUCCESS
                             : iroha::protocol::APPLY_FAILURE);
    return grpc::Status::OK;
  }

  void send_abort_genesis_block(const iroha::model::Block &block) {
    // DO NOTHING
  }
};

TEST_F(iroha_cli_test, NormalWhenParseTrustedPeers) {
  auto test_path = "/tmp/_boot_strap_test_target.conf";
  std::ofstream ofs(test_path);
  ofs << R"({"peers":[
    {"pubkey":")" +
             rnd_hex_pk() + R"(", "ip":"192.168.0.3"},
    {"pubkey":")" + rnd_hex_pk() +
             R"(", "ip":"192.168.0.4"},
    {"pubkey":")" +
             rnd_hex_pk() + R"(", "ip":"192.168.0.5"},
    {"pubkey":")" + rnd_hex_pk() +
             R"(", "ip":"192.168.0.6"}
  ]
}
)";
  ofs.close();

  MockGenesisBlockClient client_mock;
  iroha_cli::BootstrapNetwork bootstrap(client_mock);
  auto peers = bootstrap.parse_trusted_peers(test_path);
  ASSERT_EQ(4, peers.size());
  ASSERT_STREQ("192.168.0.3", peers[0].address.c_str());
  ASSERT_STREQ("192.168.0.4", peers[1].address.c_str());
  ASSERT_STREQ("192.168.0.5", peers[2].address.c_str());
  ASSERT_STREQ("192.168.0.6", peers[3].address.c_str());
  ASSERT_EQ(0, remove(test_path));
}

TEST_F(iroha_cli_test, WrongIPNameWhenParseTrustedPeers) {
  auto test_path = "/tmp/_bootstrap_test_target.conf";
  std::ofstream ofs(test_path);
  ofs << R"({"peers":[{"pubkey":")" + rnd_hex_pk() +
      R"(", "Ip":"192.168.0.5"}]})";

  MockGenesisBlockClient client_mock;
  iroha_cli::BootstrapNetwork bootstrap(client_mock);
  ASSERT_ANY_THROW({ // todo fix
                     bootstrap.parse_trusted_peers(test_path);
                   });
  ASSERT_EQ(0, remove(test_path));
}

TEST_F(iroha_cli_test, InvalidIPValueWhenParseTrustedPeers) {
  auto test_path = "/tmp/_bootstrap_test_target.conf";
  std::ofstream ofs(test_path);
  ofs << R"({"peers":[{"pubkey":")" + rnd_hex_pk() +
      R"(", "ip":"192.256.0.5"}]})";
  ofs.close();

  MockGenesisBlockClient client_mock;
  iroha_cli::BootstrapNetwork bootstrap(client_mock);
  ASSERT_ANY_THROW({ bootstrap.parse_trusted_peers(test_path); });
  ASSERT_TRUE(remove(test_path) == 0);
}

TEST_F(iroha_cli_test, NormalWhenParseGenesisBlock) {
  auto test_path = "/tmp/_bootstrap_test_genesis.json";
  std::ofstream ofs(test_path);

  ofs << std::string(R"({
  "transactions":[
    {
      "signatures":[{"signature":"blob", "pubkey":"blob"}],
      "created_ts":1111111111,
      "creator_account_id":"first_account_id",
      "tx_counter":1234567,
      "commands":[
        {
          "command_type":"CreateDomain",
          "domain_name":"soramitsu"
        },
        {
          "command_type":"CreateAccount",
          "pubkey":"blob",
          "account_name":"someone",
          "domain_id":"soramitsu"
        }
      ]
    },
    {
      "signatures":[{"signature":"blob","pubkey":"blob"}],
      "created_ts":1111111111,
      "creator_account_id":"second_account_id",
      "tx_counter":9876543,
      "commands":[
        {
          "command_type":"CreateDomain",
          "domain_name":"mydomain"
        }
      ]
    }
  ]
}
)");
  ofs.close();

  MockGenesisBlockClient client_mock;
  iroha_cli::BootstrapNetwork bootstrap(client_mock);
  auto genesis = bootstrap.parse_genesis_block(test_path);
  ASSERT_EQ(0, remove(test_path));
}

TEST_F(iroha_cli_test, MergeAddTrustedPeersWhenCreatingGenesisBlock) {
  auto block = create_genesis_block();
  MockGenesisBlockClient client_mock;
  iroha_cli::BootstrapNetwork bootstrap(client_mock);
  std::vector<iroha::model::Peer> peers(2);
  peers[0].address = "192.168.0.3";
  peers[0].pubkey = iroha::create_keypair(iroha::create_seed()).pubkey;
  peers[1].address = "192.168.0.4";
  peers[1].pubkey = iroha::create_keypair(iroha::create_seed()).pubkey;
  block = bootstrap.merge_tx_add_trusted_peers(block, peers);
  auto cmd = [&block](int idx) {
    return static_cast<iroha::model::AddPeer *>(
        block.transactions[0].commands[idx].get());
  };
  ASSERT_STREQ(cmd(0)->address.c_str(), peers[0].address.c_str());
  ASSERT_EQ(cmd(0)->peer_key, peers[0].pubkey);
  ASSERT_STREQ(cmd(1)->address.c_str(), peers[1].address.c_str());
  ASSERT_EQ(cmd(1)->peer_key, peers[1].pubkey);
}

TEST_F(iroha_cli_test, NormalWhenRunNetwork) {
  MockGenesisBlockClient client_mock;
  iroha::protocol::ApplyGenesisBlockResponse response;

  iroha_cli::BootstrapNetwork bootstrap(client_mock);

  auto block = create_genesis_block();

  std::vector<iroha::model::Peer> peers(2);
  peers[0].address = "192.168.0.3";
  peers[0].pubkey = iroha::create_keypair(iroha::create_seed()).pubkey;
  peers[1].address = "192.168.0.4";
  peers[1].pubkey = iroha::create_keypair(iroha::create_seed()).pubkey;

  ASSERT_NO_THROW({
                    bootstrap.run_network(peers, block);
                  });
}
