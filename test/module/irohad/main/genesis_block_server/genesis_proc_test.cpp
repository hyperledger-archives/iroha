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

#include "module/irohad/ametsuchi/ametsuchi_fixture.hpp"

#include "ametsuchi/impl/storage_impl.hpp"
#include "main/genesis_block_server/genesis_block_server.hpp"
#include "model/model_hash_provider_impl.hpp"
#include "model/commands/create_account.hpp"
#include "model/converters/pb_command_factory.hpp"

using namespace iroha;
using namespace iroha::ametsuchi;
using namespace iroha::model;

class GenesisBlockProcessorTest : public AmetsuchiTest {
 public:
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
    HashProviderImpl hash_provider;
    block.hash = hash_provider.get_hash(block);
    return block;
  }
 protected:
  void SetUp() override {
    block_store_path = "/tmp/test_genesis_block";
    AmetsuchiTest::SetUp();
  }
};

TEST_F(GenesisBlockProcessorTest, genesis_block_handle) {
  auto storage = StorageImpl::create(block_store_path, redishost_,
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

