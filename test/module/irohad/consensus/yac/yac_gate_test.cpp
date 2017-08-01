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

#include <gmock/gmock.h>
#include <memory>
#include "yac_mocks.hpp"
#include <rxcpp/rx.hpp>
#include <rxcpp/rx-observable.hpp>
#include "consensus/yac/impl/yac_gate_impl.hpp"
#include <common/test_observable.hpp>

using namespace iroha::consensus::yac;
using namespace common::test_observable;

#include <iostream>
using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::An;
using ::testing::AtLeast;

class BlockCreatorStub : public iroha::simulator::BlockCreator {
 public:
  void process_verified_proposal(iroha::model::Proposal proposal) override {
    // nothing to do
  };

  rxcpp::observable<iroha::model::Block> on_block() override {
    return subject.get_observable();
  };

  rxcpp::subjects::subject<iroha::model::Block> subject;

  BlockCreatorStub() = default;

  BlockCreatorStub(const BlockCreatorStub &rhs) {
  };

  BlockCreatorStub(BlockCreatorStub &&rhs) : subject(rhs.subject) {
  };

  BlockCreatorStub &operator=(const BlockCreatorStub &rhs) {
    return *this;
  };
};

TEST(YacGateTest, YacGateSubscribtionTest) {
  cout << "----------| BlockCreator (block)=> YacHate (vote)=> "
      "HashGate (commit) => YacGate => on_commit() |----------" << endl;

  // expected values
  YacHash expected_hash("proposal", "block");
  iroha::model::Block expected_block;
  expected_block.created_ts = 100500;
  VoteMessage message;
  message.hash = expected_hash;
  CommitMessage commit_message({message});
  auto expected_commit = rxcpp::observable<>::just(commit_message);

  // yac consensus
  unique_ptr<HashGate> hash_gate =
      make_unique<HashGateMock>();
  auto hash_gate_raw = hash_gate.get();

  EXPECT_CALL(*static_cast<HashGateMock *>(hash_gate_raw),
              vote(expected_hash, _)).Times(1);

  EXPECT_CALL(*static_cast<HashGateMock *>(hash_gate_raw),
              on_commit()).WillOnce(Return(expected_commit));

  // generate order of peers
  unique_ptr<YacPeerOrderer> peer_orderer =
      make_unique<YacPeerOrdererMock>();
  auto peer_orderer_raw = peer_orderer.get();

  EXPECT_CALL(*static_cast<YacPeerOrdererMock *>(peer_orderer_raw),
              getOrdering(_))
      .WillOnce(Return(ClusterOrdering()));

  // make hash from block
  shared_ptr<YacHashProvider> hash_provider =
      make_shared<YacHashProviderMock>();

  EXPECT_CALL(
      *static_cast<YacHashProviderMock *>(hash_provider.get()), makeHash(_))
      .WillOnce(Return(expected_hash));

  // make blocks
  shared_ptr<iroha::simulator::BlockCreator> block_creator =
      make_shared<BlockCreatorStub>();

  YacGateImpl gate(std::move(hash_gate), std::move(peer_orderer),
                   hash_provider, block_creator);

  // verify that yac gate subscribed for block_creator
  TestObservable<iroha::model::Block> block_wrapper(block_creator->on_block());
  block_wrapper.test_subscriber(
      std::make_unique<CallExact<iroha::model::Block>>
          (CallExact<iroha::model::Block>(1)), [](auto block) {});

  auto val = static_cast<BlockCreatorStub *>(block_creator.get());

  // initialize chain
  val->subject.get_subscriber().on_next(expected_block);
  ASSERT_EQ(true, block_wrapper.validate());

  // verify that yac gate emit expected block
  TestObservable<iroha::model::Block> gate_wrapper(gate.on_commit());
  gate_wrapper.test_subscriber(
      std::make_unique<CallExact<iroha::model::Block>>
          (CallExact<iroha::model::Block>(1)), [expected_block](auto block) {
        ASSERT_EQ(block, expected_block);
      });

  ASSERT_EQ(true, gate_wrapper.validate());
}

TEST(YacGateTest, YacGateSubscribtionTestFailCase) {
  cout << "----------| Fail case of retrieving cluster order  |----------"
       << endl;

  // expected values
  YacHash expected_hash("proposal", "block");
  iroha::model::Block expected_block;
  expected_block.created_ts = 100500;
  VoteMessage message;
  message.hash = expected_hash;
  CommitMessage commit_message({message});
  auto expected_commit = rxcpp::observable<>::just(commit_message);

  // yac consensus
  unique_ptr<HashGate> hash_gate =
      make_unique<HashGateMock>();
  auto hash_gate_raw = hash_gate.get();

  EXPECT_CALL(*static_cast<HashGateMock *>(hash_gate_raw),
              vote(_, _)).Times(0);

  EXPECT_CALL(*static_cast<HashGateMock *>(hash_gate_raw),
              on_commit()).Times(0);

  // generate order of peers
  unique_ptr<YacPeerOrderer> peer_orderer =
      make_unique<YacPeerOrdererMock>();
  auto peer_orderer_raw = peer_orderer.get();

  EXPECT_CALL(*static_cast<YacPeerOrdererMock *>(peer_orderer_raw),
              getOrdering(_))
      .WillOnce(Return(nonstd::nullopt));

  // make hash from block
  shared_ptr<YacHashProvider> hash_provider =
      make_shared<YacHashProviderMock>();

  EXPECT_CALL(
      *static_cast<YacHashProviderMock *>(hash_provider.get()), makeHash(_))
      .WillOnce(Return(expected_hash));

  // make blocks
  shared_ptr<iroha::simulator::BlockCreator> block_creator =
      make_shared<BlockCreatorStub>();

  YacGateImpl gate(std::move(hash_gate), std::move(peer_orderer),
                   hash_provider, block_creator);

  // verify that yac gate subscribed for block_creator
  TestObservable<iroha::model::Block> block_wrapper(block_creator->on_block());
  block_wrapper.test_subscriber(
      std::make_unique<CallExact<iroha::model::Block>>
          (CallExact<iroha::model::Block>(1)), [](auto block) {});

  auto val = static_cast<BlockCreatorStub *>(block_creator.get());

  // initialize chain
  val->subject.get_subscriber().on_next(expected_block);
  ASSERT_EQ(true, block_wrapper.validate());
}
