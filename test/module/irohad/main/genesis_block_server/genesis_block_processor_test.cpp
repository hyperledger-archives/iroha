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
#include <sys/stat.h>
#include <dirent.h>
#include "main/genesis_block_server/genesis_block_service.hpp"
#include "main/genesis_block_server/genesis_block_processor_impl.hpp"
#include <cpp_redis/cpp_redis>
#include <pqxx/pqxx>
#include "ametsuchi/impl/storage_impl.hpp"
#include "common/types.hpp"
#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/transfer_asset.hpp"
#include "model/model_hash_provider_impl.hpp"
#include "../../ametsuchi/ametsuchi_test_common.hpp"

using namespace iroha;

class GenesisBlockProcessorTest : public ::testing::Test {
public:
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

    remove_all(block_store_path);
  }

  std::string pgopt_ =
    "host=localhost port=5432 user=postgres password=mysecretpassword";

  std::string redishost_ = "localhost";
  size_t redisport_ = 6379;

  std::string block_store_path = "/tmp/test_genesis_block";
};

TEST_F(GenesisBlockProcessorTest, process) {
  auto storage = ametsuchi::StorageImpl::create(block_store_path, redishost_, redisport_, pgopt_);
  GenesisBlockProcessorImpl processor_impl(*storage);
  GenesisBlockService service(processor_impl);
}
