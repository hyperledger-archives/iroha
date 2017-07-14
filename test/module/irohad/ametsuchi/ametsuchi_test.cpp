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
#include <ametsuchi/impl/storage_impl.hpp>
#include <common/types.hpp>
#include <cpp_redis/cpp_redis>
#include <model/commands/create_account.hpp>
#include <model/commands/create_domain.hpp>
#include <pqxx/pqxx>

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
            "DROP TABLE IF EXISTS signatory;\n"
            "DROP SEQUENCE IF EXISTS peer_peer_id_seq;";

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
      auto wsv = storage->createTemporaryWsv();
      model::Transaction txn;
      model::CreateDomain createDomain;
      createDomain.domain_name = "ru";
      model::CreateAccount createAccount;
      createAccount.account_name = "username";
      createAccount.domain_id = "ru";
      txn.commands.push_back(
          std::make_shared<model::CreateDomain>(createDomain));
      txn.commands.push_back(
          std::make_shared<model::CreateAccount>(createAccount));
      wsv->apply(txn, [](auto &tx, auto &executor, auto &query) {
        EXPECT_TRUE(tx.commands.at(0)->execute(query, executor));
        EXPECT_TRUE(tx.commands.at(1)->execute(query, executor));
        return true;
      });
      auto account = wsv->getAccount("username@ru");
      ASSERT_TRUE(account);
      ASSERT_EQ(account->account_id, "username@ru");
      ASSERT_EQ(account->domain_name, "ru");
      ASSERT_EQ(account->master_key, createAccount.pubkey);
      ASSERT_TRUE(wsv);
    }

  }  // namespace ametsuchi
}  // namespace iroha