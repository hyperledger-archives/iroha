/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
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

#include "builders/protobuf/transport_builder.hpp"
#include <gtest/gtest.h>
#include <utility>
#include "block.pb.h"
#include "test_block_builder.hpp"
#include "test_proposal_builder.hpp"
#include "test_query_builder.hpp"
#include "test_transaction_builder.hpp"

using namespace shared_model;

class TransportBuilderTest : public ::testing::Test {
 public:
  void SetUp() override {
    created_time = iroha::time::now();
    account_id = "account@domain";
    quorum = 2;
    counter = 1048576;
    hash = std::string(32, '0');
    height = 1;
  }

  Transaction createTransaction() {
    TestTransactionBuilder builder;
    auto tx = builder.createdTime(created_time)
                  .txCounter(counter)
                  .creatorAccountId(account_id)
                  .setAccountQuorum(account_id, quorum)
                  .build();
    return tx;
  }

  Query createQuery() {
    TestQueryBuilder builder;

    auto query = builder.createdTime(created_time)
                     .creatorAccountId(account_id)
                     .getAccount(account_id)
                     .queryCounter(counter)
                     .build();
    return query;
  }

  Block createBlock() {
    TestBlockBuilder builder;

    auto block =
        builder.transactions(std::vector<Transaction>({createTransaction()}))
            .txNumber(1)
            .height(1)
            .prevHash(crypto::Hash("asd"))
            .createdTime(created_time)
            .build();
    return block;
  }

  Proposal createProposal() {
    TestProposalBuilder builder;

    auto proposal =
        builder.transactions(std::vector<Transaction>({createTransaction()}))
            .height(1)
            .createdTime(created_time)
            .build();

    return proposal;
  }

  decltype(iroha::time::now()) created_time;
  std::string account_id;
  uint8_t quorum;
  uint64_t counter;
  std::string hash;
  uint64_t height;
};

/**
 * @given valid proto object of transaction
 * @when transport builder constructs model object from it
 * @then original and built objects are equal
 */
TEST_F(TransportBuilderTest, TransactionCreationTest) {
  auto orig_tx = createTransaction();
  auto proto_tx = orig_tx.getTransport();

  TransportBuilder<proto::Transaction, validation::DefaultTransactionValidator>
      transport_builder(proto_tx);

  auto built_tx = transport_builder.build();
  ASSERT_EQ(built_tx.getTransport().SerializeAsString(),
            orig_tx.getTransport().SerializeAsString());
}

/**
 * @given valid proto object of query
 * @when transport builder constructs model object from it
 * @then original and built objects are equal
 */
TEST_F(TransportBuilderTest, QueryCreationTest) {
  auto orig_query = createQuery();
  auto proto_query = orig_query.getTransport();

  TransportBuilder<proto::Query, validation::DefaultQueryValidator>
      transport_builder(proto_query);

  auto built_query = transport_builder.build();
  ASSERT_EQ(built_query.getTransport().SerializeAsString(),
            orig_query.getTransport().SerializeAsString());
}

/**
 * @given valid proto object of block
 * @when transport builder constructs model object from it
 * @then original and built objects are equal
 */
TEST_F(TransportBuilderTest, BlockCreationTest) {
  auto orig_block = createBlock();
  auto proto_block = orig_block.getTransport();

  TransportBuilder<proto::Block, validation::DefaultBlockValidator>
      transport_builder(proto_block);

  auto built_block = transport_builder.build();
  ASSERT_EQ(built_block.getTransport().SerializeAsString(),
            orig_block.getTransport().SerializeAsString());
}

/**
 * @given valid proto object of proposal
 * @when transport builder constructs model object from it
 * @then original and built objects are equal
 */
TEST_F(TransportBuilderTest, ProposalCreationTest) {
  auto orig_proposal = createProposal();
  auto proto_proposal = orig_proposal.getTransport();

  TransportBuilder<proto::Proposal, validation::DefaultProposalValidator>
      transport_builder(proto_proposal);

  auto built_proposal = transport_builder.build();
  ASSERT_EQ(built_proposal.getTransport().SerializeAsString(),
            orig_proposal.getTransport().SerializeAsString());
}
