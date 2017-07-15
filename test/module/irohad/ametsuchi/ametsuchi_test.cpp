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
#include <cpp_redis/cpp_redis>
#include <model/commands/add_asset_quantity.hpp>
#include <model/commands/add_peer.hpp>
#include <model/commands/transfer_asset.hpp>
#include <pqxx/pqxx>
#include "ametsuchi/impl/storage_impl.hpp"
#include "common/types.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"

namespace iroha {
  namespace ametsuchi {

    class AmetsuchiTest : public ::testing::Test {
     protected:
      virtual void SetUp() {
        mkdir(block_store_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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

        //        remove_all(block_store_path);
      }

      std::string pgopt_ =
          "host=localhost port=5432 user=postgres password=mysecretpassword";

      std::string redishost_ = "localhost";
      size_t redisport_ = 6379;

      std::string block_store_path = "/tmp/block_store";
    };

    TEST_F(AmetsuchiTest, SampleTest) {
      auto storage =
          StorageImpl::create(block_store_path, redishost_, redisport_, pgopt_);
      ASSERT_TRUE(storage);

      model::Transaction txn;
      model::CreateDomain createDomain;
      createDomain.domain_name = "ru";
      txn.commands.push_back(
          std::make_shared<model::CreateDomain>(createDomain));
      model::CreateAccount createAccount;
      createAccount.account_name = "user1";
      createAccount.domain_id = "ru";
      txn.commands.push_back(
          std::make_shared<model::CreateAccount>(createAccount));

      {
        auto wsv = storage->createTemporaryWsv();
        ASSERT_TRUE(wsv);
        wsv->apply(txn, [](auto &tx, auto &executor, auto &query) {
          EXPECT_TRUE(tx.commands.at(0)->execute(query, executor));
          EXPECT_TRUE(tx.commands.at(1)->execute(query, executor));
          return true;
        });
        auto account = wsv->getAccount("user1@ru");
        ASSERT_TRUE(account);
        ASSERT_EQ(account->account_id, "user1@ru");
        ASSERT_EQ(account->domain_name, "ru");
        ASSERT_EQ(account->master_key, createAccount.pubkey);
      }

      {
        auto account = storage->getAccount("username@ru");
        ASSERT_FALSE(account);
      }

      model::Block block;
      block.transactions.push_back(txn);

      {
        auto ms = storage->createMutableStorage();
        ms->apply(block, [](const auto &blk, auto &executor, auto &query,
                            const auto &top_block) {
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(0)->execute(query, executor));
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(1)->execute(query, executor));
          return true;
        });
        storage->commit(std::move(ms));
      }

      {
        auto account = storage->getAccount("user1@ru");
        ASSERT_TRUE(account);
        ASSERT_EQ(account->account_id, "user1@ru");
        ASSERT_EQ(account->domain_name, "ru");
        ASSERT_EQ(account->master_key, createAccount.pubkey);
      }

      txn = model::Transaction();
      createAccount = model::CreateAccount();
      createAccount.account_name = "user2";
      createAccount.domain_id = "ru";
      txn.commands.push_back(
          std::make_shared<model::CreateAccount>(createAccount));
      model::CreateAsset createAsset;
      createAsset.domain_id = "ru";
      createAsset.asset_name = "RUB";
      createAsset.precision = 2;
      txn.commands.push_back(std::make_shared<model::CreateAsset>(createAsset));
      model::AddAssetQuantity addAssetQuantity;
      addAssetQuantity.asset_id = "RUB#ru";
      addAssetQuantity.account_id = "user1@ru";
      addAssetQuantity.amount = std::decimal::make_decimal64(150ull, -2);
      txn.commands.push_back(
          std::make_shared<model::AddAssetQuantity>(addAssetQuantity));
      model::TransferAsset transferAsset;
      transferAsset.src_account_id = "user1@ru";
      transferAsset.dest_account_id = "user2@ru";
      transferAsset.asset_id = "RUB#ru";
      transferAsset.amount = std::decimal::make_decimal64(100ull, -2);
      txn.commands.push_back(
          std::make_shared<model::TransferAsset>(transferAsset));

      block = model::Block();
      block.transactions.push_back(txn);

      {
        auto ms = storage->createMutableStorage();
        ms->apply(block, [](const auto &blk, auto &executor, auto &query,
                            const auto &top_block) {
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(0)->execute(query, executor));
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(1)->execute(query, executor));
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(2)->execute(query, executor));
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(3)->execute(query, executor));
          return true;
        });
        storage->commit(std::move(ms));
      }

      {
        auto asset1 = storage->getAccountAsset("user1@ru", "RUB#ru");
        ASSERT_TRUE(asset1);
        ASSERT_EQ(asset1->account_id, "user1@ru");
        ASSERT_EQ(asset1->asset_id, "RUB#ru");
        ASSERT_EQ(asset1->balance, 50);
        auto asset2 = storage->getAccountAsset("user2@ru", "RUB#ru");
        ASSERT_TRUE(asset2);
        ASSERT_EQ(asset2->account_id, "user2@ru");
        ASSERT_EQ(asset2->asset_id, "RUB#ru");
        ASSERT_EQ(asset2->balance, 100);
      }
    }

    TEST_F(AmetsuchiTest, PeerTest) {
      auto storage =
          StorageImpl::create(block_store_path, redishost_, redisport_, pgopt_);
      ASSERT_TRUE(storage);

      model::Transaction txn;
      model::AddPeer addPeer;
      addPeer.peer_key.at(0) = 1;
      addPeer.address = "192.168.0.1:50051";
      txn.commands.push_back(std::make_shared<model::AddPeer>(addPeer));

      model::Block block;
      block.transactions.push_back(txn);

      {
        auto ms = storage->createMutableStorage();
        ms->apply(block, [](const auto &blk, auto &executor, auto &query,
                            const auto &top_block) {
          EXPECT_TRUE(
              blk.transactions.at(0).commands.at(0)->execute(query, executor));
          return true;
        });
        storage->commit(std::move(ms));
      }

      auto peers = storage->getPeers();
      ASSERT_EQ(peers.size(), 1);
      ASSERT_EQ(peers.at(0).pubkey, addPeer.peer_key);
      ASSERT_EQ(peers.at(0).address, addPeer.address);
    }

  }  // namespace ametsuchi
}  // namespace iroha