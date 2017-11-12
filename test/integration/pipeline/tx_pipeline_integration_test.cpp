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

#include "crypto/hash.hpp"
#include "crypto/keys_manager_impl.hpp"
#include "datetime/time.hpp"
#include "framework/test_subscriber.hpp"
#include "main/application.hpp"
#include "main/raw_block_insertion.hpp"
#include "model/generators/block_generator.hpp"
#include "module/irohad/ametsuchi/ametsuchi_fixture.hpp"

#include <cstdio>
#include <model/commands/append_role.hpp>

using namespace framework::test_subscriber;
using namespace std::chrono_literals;
using namespace iroha::model::generators;
using iroha::model::Transaction;

class TestIrohad : public Irohad {
 public:
  TestIrohad(const std::string &block_store_dir,
             const std::string &redis_host,
             size_t redis_port,
             const std::string &pg_conn,
             size_t torii_port,
             size_t internal_port,
             size_t max_proposal_size,
             std::chrono::milliseconds proposal_delay,
             std::chrono::milliseconds vote_delay,
             std::chrono::milliseconds load_delay,
             const iroha::keypair_t &keypair)
      : Irohad(block_store_dir,
               redis_host,
               redis_port,
               pg_conn,
               torii_port,
               internal_port,
               max_proposal_size,
               proposal_delay,
               vote_delay,
               load_delay,
               keypair) {}

  auto &getCommandService() { return command_service; }

  auto &getPeerCommunicationService() { return pcs; }

  auto &getCryptoProvider() { return crypto_verifier; }

  void run() override {
    grpc::ServerBuilder builder;
    int port = 0;
    builder.AddListeningPort("0.0.0.0:" + std::to_string(internal_port_),
                             grpc::InsecureServerCredentials(),
                             &port);
    builder.RegisterService(ordering_init.ordering_gate_transport.get());
    builder.RegisterService(ordering_init.ordering_service_transport.get());
    builder.RegisterService(yac_init.consensus_network.get());
    builder.RegisterService(loader_init.service.get());
    internal_server = builder.BuildAndStart();
    internal_thread = std::thread([this] { internal_server->Wait(); });
    log_->info("===> iroha initialized");
  }
};

class TxPipelineIntegrationTest : public iroha::ametsuchi::AmetsuchiTest {
 public:
  TxPipelineIntegrationTest() {
    //spdlog::set_level(spdlog::level::off);
  }

  void SetUp() override {
    iroha::ametsuchi::AmetsuchiTest::SetUp();
    genesis_block =
        iroha::model::generators::BlockGenerator().generateGenesisBlock(
            {"0.0.0.0:10001"});
    manager = std::make_shared<iroha::KeysManagerImpl>("node0");
    auto keypair = manager->loadKeys().value();

    irohad = std::make_shared<TestIrohad>(block_store_path,
                                          redishost_,
                                          redisport_,
                                          pgopt_,
                                          0,
                                          10001,
                                          10,
                                          5000ms,
                                          5000ms,
                                          5000ms,
                                          keypair);

    ASSERT_TRUE(irohad->storage);

    // insert genesis block
    iroha::main::BlockInserter inserter(irohad->storage);


    inserter.applyToLedger({genesis_block});

    // initialize irohad
    irohad->init();

    // start irohad
    irohad->run();
  }

  void TearDown() override {
    iroha::ametsuchi::AmetsuchiTest::TearDown();
    std::remove("node0.pub");
    std::remove("node0.priv");
    std::remove("admin@test.pub");
    std::remove("admin@test.priv");
    std::remove("test@test.pub");
    std::remove("test@test.priv");
    std::remove("nba@usabnk.pub");
    std::remove("nba@usabnk.priv");
    std::remove("ivan@ru.pub");
    std::remove("ivan@ru.priv");
    std::remove("tea@khm.pub");
    std::remove("tea@khm.priv");
  }

  void sendTransactions(std::vector<iroha::model::Transaction> transactions) {
    // generate expected proposal
    expected_proposal = std::make_shared<iroha::model::Proposal>(transactions);
    expected_proposal->height = 2;

    // generate expected block
    expected_block = iroha::model::Block{};
    expected_block.height = expected_proposal->height;
    expected_block.prev_hash = genesis_block.hash;
    expected_block.transactions = transactions;
    expected_block.txs_number = transactions.size();
    expected_block.created_ts = 0;
    expected_block.merkle_root.fill(0);
    expected_block.hash = iroha::hash(expected_block);
    irohad->getCryptoProvider()->sign(expected_block);

    // send transactions to torii
    for (const auto &tx : transactions) {
      auto pb_tx =
          iroha::model::converters::PbTransactionFactory().serialize(tx);

      google::protobuf::Empty response;
      irohad->getCommandService()->ToriiAsync(pb_tx, response);
    }
  }

  void validate() {
    // verify proposal
    auto proposal_wrapper = make_test_subscriber<CallExact>(
        irohad->getPeerCommunicationService()->on_proposal(), 1);
    proposal_wrapper.subscribe(
        [this](auto proposal) { proposals.push_back(proposal); });

    // verify commit and block
    auto commit_wrapper = make_test_subscriber<CallExact>(
        irohad->getPeerCommunicationService()->on_commit(), 1);
    commit_wrapper.subscribe([this](auto commit) {
      auto block_wrapper = make_test_subscriber<CallExact>(commit, 1);
      block_wrapper.subscribe([this](auto block) { blocks.push_back(block); });
    });
    irohad->getPeerCommunicationService()->on_commit().subscribe(
        [this](auto) { cv.notify_one(); });

    // wait for commit
    std::unique_lock<std::mutex> lk(m);

    cv.wait_for(lk, 10s, [this] { return blocks.size() == 1; });

    ASSERT_TRUE(proposal_wrapper.validate());
    ASSERT_EQ(1, proposals.size());
    ASSERT_EQ(*expected_proposal, proposals.front());

    ASSERT_TRUE(commit_wrapper.validate());
    ASSERT_EQ(1, blocks.size());
    ASSERT_EQ(expected_block, blocks.front());
  }

  nonstd::optional<iroha::keypair_t> createNewAccountKeypair(
      const std::string& accountName,
      const std::string& passPhrase) const {
    auto manager = iroha::KeysManagerImpl(accountName);
    EXPECT_TRUE(manager.createKeys(passPhrase));
    return manager.loadKeys();
  }

  std::shared_ptr<TestIrohad> irohad;

  std::condition_variable cv;
  std::mutex m;

  std::shared_ptr<iroha::model::Proposal> expected_proposal;
  iroha::model::Block genesis_block, expected_block;

  std::vector<iroha::model::Proposal> proposals;
  std::vector<iroha::model::Block> blocks;

  std::shared_ptr<iroha::KeysManager> manager;
};

TEST_F(TxPipelineIntegrationTest, TxPipelineTest) {
  // generate test command
  auto cmd =
      iroha::model::generators::CommandGenerator().generateAddAssetQuantity(
          "admin@test",
          "coin#test",
          iroha::Amount().createFromString("20.00").value());

  // generate test transaction
  auto tx =
      iroha::model::generators::TransactionGenerator().generateTransaction(
          "admin@test", 1, {cmd});
  iroha::KeysManagerImpl manager("admin@test");
  auto keypair = manager.loadKeys().value();
  iroha::model::ModelCryptoProviderImpl provider(keypair);
  provider.sign(tx);

  sendTransactions({tx});

  validate();
}

/**
 * @given Admin creates domains: [usabnk, ru, khm]
 *        National Bank of America, creates domain usabnk (nba@usabnk) and asset usd,
 * @when A user ivan in domain of Russia (ivan@ru) wants to possess and
 *       transfer asset usd#usabnk to user tea in domain of Cambodia (tea@khm).
 * @then Validate transactions processed.
 */
TEST_F(TxPipelineIntegrationTest, TransferAssetTestAmongDomainTest) {
  const std::string DOMAIN_USABANK = "usabnk";
  const std::string DOMAIN_RU = "ru";
  const std::string DOMAIN_KHM = "khm";
  const std::string USER_DEFAULT_ROLE = "user";
  const std::string MONEY_CREATOR_ROLE = "money_creator";
  const std::string ASSET_USD_NAME = "usd";
  const std::string ASSET_USD_ID = ASSET_USD_NAME + "#" + DOMAIN_USABANK;
  const std::string NBA_NAME = "nba";
  const std::string IVAN_NAME = "ivan";
  const std::string TEA_NAME = "tea";
  const std::string ADMIN_ID = "admin@test";
  const std::string NBA_ID = NBA_NAME + "@" + DOMAIN_USABANK;
  const std::string IVAN_ID = IVAN_NAME + "@" + DOMAIN_RU;
  const std::string TEA_ID = TEA_NAME + "@" + DOMAIN_KHM;

  auto adminKeypair = iroha::KeysManagerImpl(ADMIN_ID).loadKeys().value();
  auto nbaKeypair = createNewAccountKeypair(NBA_ID, "nba_pass").value();
  auto ivanKeypair = createNewAccountKeypair(IVAN_ID, "ivan_pass").value();
  auto teaKeypair = createNewAccountKeypair(TEA_ID, "tea_pass").value();

  // Admin creates domains: [usabnk, khm, ru]
  auto tx1 = TransactionGenerator().generateTransaction(
    ADMIN_ID, 1, {
      CommandGenerator().generateCreateDomain(
        DOMAIN_USABANK, MONEY_CREATOR_ROLE),
      CommandGenerator().generateCreateDomain(
        DOMAIN_RU, USER_DEFAULT_ROLE),
      CommandGenerator().generateCreateDomain(
        DOMAIN_KHM, USER_DEFAULT_ROLE),
      CommandGenerator().generateCreateAccount(
        NBA_NAME, DOMAIN_USABANK, nbaKeypair.pubkey),
      CommandGenerator().generateCreateAccount(
        IVAN_NAME, DOMAIN_RU, ivanKeypair.pubkey),
      CommandGenerator().generateCreateAccount(
        TEA_NAME, DOMAIN_KHM, teaKeypair.pubkey)
    });

  iroha::model::ModelCryptoProviderImpl(adminKeypair).sign(tx1);

  // creates default domains and usd asset. add asset qty to nba
  auto tx2 = TransactionGenerator().generateTransaction(
    NBA_ID, 1, {
      CommandGenerator().generateCreateAsset(
        ASSET_USD_NAME, DOMAIN_USABANK, 2),
      CommandGenerator().generateAddAssetQuantity(
        NBA_ID, ASSET_USD_ID,
        iroha::Amount().createFromString("1000000.00").value())
    });

  iroha::model::ModelCryptoProviderImpl(nbaKeypair).sign(tx2);

  // transfer asset to ivan@ru and tea@khm
  auto tx3 = TransactionGenerator().generateTransaction(
    NBA_ID, 1, {
      CommandGenerator().generateTransferAsset(
        NBA_ID, IVAN_ID, ASSET_USD_ID,
        iroha::Amount().createFromString("30.50").value()),
      CommandGenerator().generateTransferAsset(
        NBA_ID, TEA_ID, ASSET_USD_ID,
        iroha::Amount().createFromString("50.00").value())
      });

  iroha::model::ModelCryptoProviderImpl(nbaKeypair).sign(tx3);

  // transfer asset from ivan@ru to tea@khm
  auto tx4 = TransactionGenerator().generateTransaction(
    IVAN_ID, 1, {
      CommandGenerator().generateTransferAsset(
        IVAN_ID, TEA_ID, ASSET_USD_ID,
        iroha::Amount().createFromString("5.50").value())
    });

  iroha::model::ModelCryptoProviderImpl(ivanKeypair).sign(tx4);

  sendTransactions({tx1, tx2, tx3, tx4});
  validate();
}
