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
#include "synchronizer/impl/synchronizer_impl.hpp"
#include "common/test_observable.hpp"
#include "validation/chain_validator.hpp"

using namespace iroha;
using namespace iroha::model;
using namespace iroha::ametsuchi;
using namespace iroha::synchronizer;
using namespace common::test_observable;

using ::testing::Return;
using ::testing::_;
using ::testing::DefaultValue;

class ChainValidatorMock : public iroha::validation::ChainValidator {
 public:
  MOCK_METHOD2(validateChain, bool(Commit, MutableStorage&));

  MOCK_METHOD2(validateBlock, bool(const Block&, MutableStorage&));
};

class MutableFactoryMock : public MutableFactory {
 public:
  MOCK_METHOD0(createMutableStorage, std::unique_ptr<MutableStorage>());

  // gmock workaround for non-copyable parameters
  void commit(std::unique_ptr<MutableStorage> mutableStorage) override {
    commit_(mutableStorage);
  }

  MOCK_METHOD1(commit_, void(std::unique_ptr<MutableStorage>&));
};

class MutableStorageMock : public MutableStorage {
  MOCK_METHOD2(apply,
               bool(const Block &,
                    std::function<bool(const Block &, WsvCommand &,
                                       WsvQuery &, const hash256_t &)>));
  MOCK_METHOD1(getAccount,
               nonstd::optional<Account>(const std::string &account_id));
  MOCK_METHOD1(getSignatories, nonstd::optional<std::vector<ed25519::pubkey_t>>(
      const std::string &account_id));
  MOCK_METHOD1(getAsset,
               nonstd::optional<Asset>(const std::string &asset_id));
  MOCK_METHOD2(getAccountAsset,
               nonstd::optional<AccountAsset>(
                   const std::string &account_id, const std::string &asset_id));
  MOCK_METHOD0(getPeers, nonstd::optional<std::vector<Peer>>());
};

std::unique_ptr<MutableStorage> createMutableStorageMock() {
  return std::make_unique<MutableStorageMock>();
}

class BlockLoader : public iroha::network::BlockLoader {
 public:
  MOCK_METHOD2(requestBlocks, rxcpp::observable<Block>(Peer&, Block&));
};

TEST(SynchronizerTest, ValidWhenSingleCommitSynchronized) {
  ChainValidatorMock chain_validator;
  MutableFactoryMock mutable_factory;
  BlockLoader block_loader;

  auto synchronizer = iroha::synchronizer::SynchronizerImpl(
      chain_validator, mutable_factory, block_loader);

  Block test_block;
  test_block.height = 5;

  DefaultValue<std::unique_ptr<MutableStorage>>::SetFactory(
      &createMutableStorageMock);
  EXPECT_CALL(mutable_factory, createMutableStorage()).Times(1);

  EXPECT_CALL(mutable_factory, commit_(_)).Times(1);

  EXPECT_CALL(chain_validator, validateBlock(test_block, _))
      .WillOnce(Return(true));

  EXPECT_CALL(block_loader, requestBlocks(_, _)).Times(0);

  TestObservable<Commit> wrapper(synchronizer.on_commit_chain());
  wrapper.test_subscriber(
      std::make_unique<CallExact<Commit>>(1), [&test_block](auto commit) {
        TestObservable<Block> block_wrapper(commit);
        block_wrapper.test_subscriber(
            std::make_unique<CallExact<Block>>(1), [&test_block](auto block) {
              // Check commit block
              ASSERT_EQ(block.height, test_block.height);
            });
        ASSERT_TRUE(block_wrapper.validate());
      });

  synchronizer.process_commit(test_block);

  ASSERT_TRUE(wrapper.validate());
}

TEST(SynchronizerTest, ValidWhenBadStorage) {
  ChainValidatorMock chain_validator;
  MutableFactoryMock mutable_factory;
  BlockLoader block_loader;

  auto synchronizer = iroha::synchronizer::SynchronizerImpl(
      chain_validator, mutable_factory, block_loader);

  Block test_block;

  DefaultValue<std::unique_ptr<MutableStorage>>::Clear();
  EXPECT_CALL(mutable_factory, createMutableStorage()).Times(1);

  EXPECT_CALL(mutable_factory, commit_(_)).Times(0);

  EXPECT_CALL(chain_validator, validateBlock(test_block, _))
      .Times(0);

  EXPECT_CALL(block_loader, requestBlocks(_, _)).Times(0);

  TestObservable<Commit> wrapper(synchronizer.on_commit_chain());
  wrapper.test_subscriber(
      std::make_unique<CallExact<Commit>>(0), [](auto commit) {});

  synchronizer.process_commit(test_block);

  ASSERT_TRUE(wrapper.validate());
}

TEST(SynchronizerTest, ValidWhenBlockValidationFailure) {
  ChainValidatorMock chain_validator;
  MutableFactoryMock mutable_factory;
  BlockLoader block_loader;

  auto synchronizer = iroha::synchronizer::SynchronizerImpl(
      chain_validator, mutable_factory, block_loader);

  Block test_block;
  test_block.height = 5;
  test_block.sigs.emplace_back();

  DefaultValue<std::unique_ptr<MutableStorage>>::SetFactory(
      &createMutableStorageMock);
  EXPECT_CALL(mutable_factory, createMutableStorage()).Times(2);

  EXPECT_CALL(mutable_factory, commit_(_)).Times(1);

  EXPECT_CALL(chain_validator, validateBlock(test_block, _))
      .WillOnce(Return(false));
  EXPECT_CALL(chain_validator, validateChain(_, _))
      .WillOnce(Return(true));

  EXPECT_CALL(block_loader, requestBlocks(_, _))
      .WillOnce(Return(rxcpp::observable<>::just(test_block)));

  TestObservable<Commit> wrapper(synchronizer.on_commit_chain());
  wrapper.test_subscriber(
      std::make_unique<CallExact<Commit>>(1), [&test_block](auto commit) {
        TestObservable<Block> block_wrapper(commit);
        block_wrapper.test_subscriber(
            std::make_unique<CallExact<Block>>(1), [&test_block](auto block) {
              // Check commit block
              ASSERT_EQ(block.height, test_block.height);
            });
        ASSERT_TRUE(block_wrapper.validate());
      });

  synchronizer.process_commit(test_block);

  ASSERT_TRUE(wrapper.validate());
}