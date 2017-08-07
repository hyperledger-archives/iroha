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

#include <dirent.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <cpp_redis/cpp_redis>
#include <pqxx/pqxx>
#include <main/genesis_block_server/genesis_block_server.hpp>
#include "common/files.hpp"
#include "ametsuchi/impl/storage_impl.hpp"
#include "main/genesis_block_server/genesis_block_processor.hpp"
#include "model/model_hash_provider_impl.hpp"
#include "model/account.hpp"
#include "model/commands/create_account.hpp"
#include "crypto/crypto.hpp"
#include "model/converters/pb_command_factory.hpp"

using namespace iroha;

class GenesisBlockProcessorTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    mkdir(block_store_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    auto pg_host = std::getenv("IROHA_POSTGRES_HOST");
    auto pg_port = std::getenv("IROHA_POSTGRES_PORT");
    auto pg_user = std::getenv("IROHA_POSTGRES_USER");
    auto pg_pass = std::getenv("IROHA_POSTGRES_PASSWORD");
    auto rd_host = std::getenv("IROHA_REDIS_HOST");
    auto rd_port = std::getenv("IROHA_REDIS_PORT");
    if (!pg_host) {
      return;
    }
    std::stringstream ss;
    ss << "host=" << pg_host << " port=" << pg_port << " user=" << pg_user
       << " password=" << pg_pass;
    pgopt_ = ss.str();
    redishost_ = rd_host;
    redisport_ = std::stoull(rd_port);
  }
  virtual void TearDown() {
    const auto drop =
        "DROP TABLE IF EXISTS account_has_asset;\n"
        "DROP TABLE IF EXISTS account_has_signatory;\n"
        "DROP TABLE IF EXISTS peer;\n"
        "DROP TABLE IF EXISTS account;\n"
        "DROP TABLE IF EXISTS exchange;\n"
        "DROP TABLE IF EXISTS asset;\n"
        "DROP TABLE IF EXISTS domain;\n"
        "DROP TABLE IF EXISTS signatory;";

    pqxx::connection connection(pgopt_);
    pqxx::work txn(connection);
    txn.exec(drop);
    txn.commit();
    connection.disconnect();

    cpp_redis::redis_client client;
    client.connect(redishost_, redisport_);
    client.flushall();
    client.sync_commit();
    client.disconnect();

    remove_all(block_store_path);
  }

  static iroha::model::Block create_genesis_block() {
    iroha::model::Transaction tx;
    tx.created_ts = 111111;
    tx.tx_counter = 987654;
    tx.creator_account_id = "admin";
    
    model::CreateDomain createDomain;
    createDomain.domain_name = "ja";
    tx.commands.push_back(
      std::make_shared<model::CreateDomain>(createDomain));

    iroha::model::CreateAccount create_account;
    auto keypair = iroha::create_keypair(iroha::create_seed("pass"));
    create_account.pubkey = keypair.pubkey;
    create_account.domain_id = "ja";
    create_account.account_name = "user";
    tx.commands.push_back(
        std::make_shared<iroha::model::CreateAccount>(create_account));

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

  std::string pgopt_ =
      "host=localhost port=5432 user=postgres password=mysecretpassword";

  std::string redishost_ = "localhost";
  size_t redisport_ = 6379;

  std::string block_store_path = "/tmp/test_genesis_block";
};

TEST_F(GenesisBlockProcessorTest, genesis_block_handle) {
  auto storage = ametsuchi::StorageImpl::create(block_store_path, redishost_,
                                                redisport_, pgopt_);
  ASSERT_TRUE(storage);

  GenesisBlockProcessor processor(*storage);
  auto block = create_genesis_block();
  ASSERT_TRUE(processor.genesis_block_handle(block));
  auto account = storage->getAccount("user@ja");
  ASSERT_STREQ(account->account_id.c_str(), "user@ja");
  ASSERT_STREQ(account->domain_name.c_str(), "ja");
  ASSERT_EQ(account->master_key, iroha::create_keypair(iroha::create_seed("pass")).pubkey);
}

