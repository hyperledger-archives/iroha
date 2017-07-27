/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <endpoint.pb.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <main/server_runner.hpp>
#include <memory>
#include <thread>
#include <torii/command_client.hpp>
#include <torii/command_service.hpp>
#include <torii/processor/query_processor_impl.hpp>
#include <torii_utils/query_client.hpp>

#include "torii/processor/transaction_processor_impl.hpp"

constexpr const char *Ip = "0.0.0.0";
constexpr int Port = 50051;

constexpr size_t TimesToriiBlocking = 5;
constexpr size_t TimesToriiNonBlocking = 5;
constexpr size_t TimesFind = 100;

using ::testing::Return;
using ::testing::A;
using ::testing::_;
using ::testing::AtLeast;

class PCSMock : public iroha::network::PeerCommunicationService {
 public:
  MOCK_METHOD1(propagate_transaction, void(iroha::model::Transaction));

  MOCK_METHOD0(on_proposal, rxcpp::observable<iroha::model::Proposal>());

  MOCK_METHOD0(on_commit, rxcpp::observable<iroha::network::Commit>());
};

class SVMock : public iroha::validation::StatelessValidator {
 public:
  MOCK_CONST_METHOD1(validate, bool(const iroha::model::Transaction &));
  MOCK_CONST_METHOD1(validate, bool(const iroha::model::Query &));
};

/**
 * Mock for wsv query
 */
class WsvQueryMock : public iroha::ametsuchi::WsvQuery {
 public:
  MOCK_METHOD1(getAccount, nonstd::optional<iroha::model::Account>(
                               const std::string &account_id));
  MOCK_METHOD1(getSignatories,
               nonstd::optional<std::vector<iroha::ed25519::pubkey_t>>(
                   const std::string &account_id));
  MOCK_METHOD1(getAsset, nonstd::optional<iroha::model::Asset>(
                             const std::string &asset_id));
  MOCK_METHOD2(getAccountAsset,
               nonstd::optional<iroha::model::AccountAsset>(
                   const std::string &account_id, const std::string &asset_id));
  MOCK_METHOD0(getPeers, nonstd::optional<std::vector<iroha::model::Peer>>());
};

/**
 * Mock for block query
 */
class BlockQueryMock : public iroha::ametsuchi::BlockQuery {
  MOCK_METHOD1(
      getAccountTransactions,
      rxcpp::observable<iroha::model::Transaction>(std::string account_id));
  MOCK_METHOD2(getBlocks, rxcpp::observable<iroha::model::Block>(uint32_t from,
                                                                 uint32_t to));
};

/**
 * Mock for query processing factory
 */
class QpfMock : public iroha::model::QueryProcessingFactory {
 public:
  MOCK_METHOD1(execute, std::shared_ptr<iroha::model::QueryResponse>(
                            const iroha::model::Query &query));
};

class ToriiServiceTest : public testing::Test {
 public:
  virtual void SetUp() {
    runner = new ServerRunner(Ip, Port);
    th = std::thread([runner = runner] {
      // ----------- Command Service --------------
      PCSMock pcsMock;
      SVMock svMock;

      EXPECT_CALL(svMock, validate(A<const iroha::model::Transaction &>()))
          .WillRepeatedly(Return(true));

      EXPECT_CALL(pcsMock, propagate_transaction(_)).Times(AtLeast(1));

      auto tx_processor =
          iroha::torii::TransactionProcessorImpl(pcsMock, svMock);
      iroha::model::converters::PbTransactionFactory pb_tx_factory;
      auto command_service =  // std::unique_ptr<torii::CommandService>(new
                              // torii::CommandService(pb_tx_factory,
                              // tx_processor));
          std::make_unique<torii::CommandService>(pb_tx_factory, tx_processor);

      //----------- Query Service ----------
      WsvQueryMock wsv_query;
      BlockQueryMock block_query;
      iroha::model::QueryProcessingFactory qpf(wsv_query, block_query);

      EXPECT_CALL(svMock, validate(A<const iroha::model::Query &>()))
          .WillRepeatedly(Return(false));

      iroha::torii::QueryProcessorImpl qpi(qpf, svMock);

      iroha::model::converters::PbQueryFactory pb_query_factory;
      iroha::model::converters::PbQueryResponseFactory pb_query_resp_factory;

      auto query_service = std::make_unique<torii::QueryService>(
          pb_query_factory, pb_query_resp_factory, qpi);

      //----------- Server run ----------------
      runner->run(std::move(command_service), std::move(query_service));
    });

    runner->waitForServersReady();
  }

  virtual void TearDown() {
    runner->shutdown();
    delete runner;
    th.join();
  }

  ServerRunner *runner;
  std::thread th;
};

TEST_F(ToriiServiceTest, ToriiWhenBlocking) {
  for (size_t i = 0; i < TimesToriiBlocking; ++i) {
    iroha::protocol::ToriiResponse response;
    // One client is generating transaction
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    meta->set_creator_account_id("accountA");
    auto stat = torii::CommandSyncClient(Ip, Port).Torii(new_tx, response);
    ASSERT_TRUE(stat.ok());
    ASSERT_EQ(response.validation(),
              iroha::protocol::STATELESS_VALIDATION_SUCCESS);
  }
}

TEST_F(ToriiServiceTest, ToriiWhenNonBlocking) {
  torii::CommandAsyncClient client(Ip, Port);
  std::atomic_int count{0};

  for (size_t i = 0; i < TimesToriiNonBlocking; ++i) {
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    meta->set_creator_account_id("accountA");

    auto stat =
        client.Torii(new_tx, [&count](iroha::protocol::ToriiResponse response) {
          ASSERT_EQ(response.validation(),
                    iroha::protocol::STATELESS_VALIDATION_SUCCESS);
          count++;
        });
  }

  while (count < (int)TimesToriiNonBlocking)
    ;
  ASSERT_EQ(count, TimesToriiNonBlocking);
}

TEST_F(ToriiServiceTest, FindWhereQueryServiceSync) {
  iroha::protocol::QueryResponse response;
  auto query = iroha::protocol::Query();
  query.set_creator_account_id("accountA");
  query.mutable_get_account()->set_account_id("accountB");
  auto stat = torii_utils::QuerySyncClient(Ip, Port).Find(query, response);
  ASSERT_TRUE(stat.ok());
  // Must return Error Response
  ASSERT_EQ(response.error_response().reason(), "Not valid");
}
/*
TEST_F(ToriiServiceTest, FindManyTimesWhereQueryServiceSync) {
  for (size_t i = 0; i < TimesFind; ++i) {
    iroha::protocol::QueryResponse response;
    auto stat = torii_utils::QuerySyncClient(Ip, Port).Find(
        iroha::protocol::Query{}, response);
    ASSERT_TRUE(stat.ok());
  }
}

TEST_F(ToriiServiceTest, MixRPCWhereCommandAndQueryService) {
  torii::CommandAsyncClient client(Ip, Port);

  for (size_t i = 0; i < TimesFind; ++i) {
    iroha::protocol::QueryResponse qresp;
    grpc::Status stat;
    stat = torii_utils::QuerySyncClient(Ip, Port).Find(iroha::protocol::Query{},
                                                       qresp);
    ASSERT_TRUE(stat.ok());
    client.Torii(iroha::protocol::Transaction{},
                 [](iroha::protocol::ToriiResponse response) {
                   ASSERT_EQ(response.code(),
                             iroha::protocol::ResponseCode::OK);
                   std::cout << "Async response\n";
                 });
    iroha::protocol::ToriiResponse response;
    stat = torii::CommandSyncClient(Ip, Port).Torii(
        iroha::protocol::Transaction{}, response);
    ASSERT_TRUE(stat.ok());
    std::cout << "Sync Response\n";
  }
}
*/