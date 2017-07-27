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

#include <gtest/gtest.h>
#include "mock_classes.hpp"
#include "main/server_runner.hpp"
#include "torii/processor/transaction_processor_impl.hpp"
#include "torii/processor/query_processor_impl.hpp"
#include "torii/command_service.hpp"
#include "torii_utils/query_client.hpp"


constexpr const char *Ip = "0.0.0.0";
constexpr int Port = 50051;

constexpr size_t TimesToriiBlocking = 5;
constexpr size_t TimesToriiNonBlocking = 5;
constexpr size_t TimesFind = 100;

using ::testing::Return;
using ::testing::A;
using ::testing::_;
using ::testing::AtLeast;


class ToriiServiceTest : public testing::Test {
 public:
  virtual void SetUp() {
    runner = new ServerRunner(Ip, Port);
    th = std::thread([runner = runner] {
      // ----------- Command Service --------------
      PCSMock pcsMock;
      SVMock svMock;

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


TEST_F(ToriiServiceTest, FindManyTimesWhereQueryServiceSync) {
  for (size_t i = 0; i < TimesFind; ++i) {
    iroha::protocol::QueryResponse response;
    auto query = iroha::protocol::Query();
    query.set_creator_account_id("accountA");
    query.mutable_get_account()->set_account_id("accountB");
    query.set_query_counter(i);

    auto stat = torii_utils::QuerySyncClient(Ip, Port).Find(query, response);
    ASSERT_TRUE(stat.ok());
    // Must return Error Response
    ASSERT_EQ(response.error_response().reason(), "Not valid");
  }
}
