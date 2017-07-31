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
#include "validation/impl/chain_validator_impl.hpp"

using namespace iroha;
using namespace iroha::model;
using namespace iroha::validation;
using namespace iroha::ametsuchi;

using ::testing::_;
using ::testing::A;
using ::testing::Return;
using ::testing::InvokeArgument;
using ::testing::ByRef;

class CryptoProviderMock : public ModelCryptoProvider {
 public:
  MOCK_CONST_METHOD1(verify, bool(const Transaction &));
  MOCK_CONST_METHOD1(verify, bool(const Query &));
  MOCK_CONST_METHOD1(verify, bool(const Block &));
};

class MutableStorageMock : public MutableStorage {
 public:

  MutableStorageMock() = default;

  MutableStorageMock(const MutableStorageMock&) = delete;

  MutableStorageMock(MutableStorageMock&&) {}

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

class WsvCommandMock : public iroha::ametsuchi::WsvCommand {
 public:

  WsvCommandMock() = default;

  WsvCommandMock(const WsvCommandMock&) = delete;

  WsvCommandMock(WsvCommandMock&&) {}

  MOCK_METHOD1(insertAccount, bool(const iroha::model::Account &));
  MOCK_METHOD1(updateAccount, bool(const iroha::model::Account &));
  MOCK_METHOD1(insertAsset, bool(const iroha::model::Asset &));
  MOCK_METHOD1(upsertAccountAsset, bool(const iroha::model::AccountAsset &));
  MOCK_METHOD1(insertSignatory, bool(const iroha::ed25519::pubkey_t &));

  MOCK_METHOD2(insertAccountSignatory,
               bool(const std::string &, const iroha::ed25519::pubkey_t &));

  MOCK_METHOD2(deleteAccountSignatory,
               bool(const std::string &, const iroha::ed25519::pubkey_t &));

  MOCK_METHOD1(insertPeer, bool(const iroha::model::Peer &));

  MOCK_METHOD1(deletePeer, bool(const iroha::model::Peer &));

  MOCK_METHOD1(insertDomain, bool(const iroha::model::Domain &));
};

class MockCommand : public Command {
 public:
  MOCK_METHOD2(validate, bool(WsvQuery &, const Account &));
  MOCK_METHOD2(execute, bool(WsvQuery &, WsvCommand &));

  MOCK_CONST_METHOD1(Equals, bool(const Command &));
  bool operator==(const Command &rhs) const override { return Equals(rhs); }

  MOCK_CONST_METHOD1(NotEquals, bool(const Command &));
  bool operator!=(const Command &rhs) const override { return NotEquals(rhs); }
};

class ChainValidationTest : public ::testing::Test {
 public:
  ChainValidationTest() : validator(provider) {}

  CryptoProviderMock provider;
  ChainValidatorImpl validator;
  MutableStorageMock storage;
};

TEST_F(ChainValidationTest, ValidWhenOnePeer) {
  EXPECT_CALL(storage, getPeers())
      .WillOnce(Return(std::vector<model::Peer>(1)));
  EXPECT_CALL(provider, verify(A<const model::Block &>()))
      .WillOnce(Return(true));

  Block block;
  block.sigs.emplace_back();

  EXPECT_CALL(storage, apply(block, _)).WillOnce(Return(true));

  ASSERT_TRUE(validator.validateBlock(block, storage));
}

TEST_F(ChainValidationTest, ValidWhenNoPeers) {
  EXPECT_CALL(storage, getPeers())
      .WillOnce(Return(std::vector<model::Peer>(0)));
  EXPECT_CALL(provider, verify(A<const model::Block &>()))
      .WillOnce(Return(true));

  Block block;
  block.sigs.emplace_back();

  EXPECT_CALL(storage, apply(block, _)).WillOnce(Return(true));

  ASSERT_TRUE(validator.validateBlock(block, storage));
}

TEST_F(ChainValidationTest, FailWhenDifferentPrevHash) {
  WsvCommandMock wsvCommand;

  EXPECT_CALL(storage, getPeers())
      .WillOnce(Return(std::vector<model::Peer>(1)));
  EXPECT_CALL(provider, verify(A<const model::Block &>()))
      .WillOnce(Return(true));

  auto cmd = std::make_shared<MockCommand>();

  Block block;
  block.sigs.emplace_back();
  block.transactions.emplace_back();
  block.transactions.front().commands.emplace_back(cmd);
  block.prev_hash.fill(1);

  hash256_t myhash;
  myhash.fill(0);

  EXPECT_CALL(storage, apply(_, _))
      .WillOnce(InvokeArgument<1>(ByRef(block), ByRef(wsvCommand),
                                  ByRef(storage), ByRef(myhash)));

  ASSERT_FALSE(validator.validateBlock(block, storage));
}

TEST_F(ChainValidationTest, ValidWhenSamePrevHash) {
  WsvCommandMock wsvCommand;

  EXPECT_CALL(storage, getPeers())
      .WillOnce(Return(std::vector<model::Peer>(1)));
  EXPECT_CALL(provider, verify(A<const model::Block &>()))
      .WillOnce(Return(true));

  auto cmd = std::make_shared<MockCommand>();

  Block block;
  block.sigs.emplace_back();
  block.transactions.emplace_back();
  block.transactions.front().commands.emplace_back(cmd);
  block.prev_hash.fill(0);

  hash256_t myhash;
  myhash.fill(0);

  EXPECT_CALL(*cmd, execute(_, _)).WillOnce(Return(true));

  EXPECT_CALL(storage, apply(_, _))
      .WillOnce(InvokeArgument<1>(ByRef(block), ByRef(wsvCommand),
                                  ByRef(storage), ByRef(myhash)));

  ASSERT_TRUE(validator.validateBlock(block, storage));
}

TEST_F(ChainValidationTest, ValidWhenValidateChainFromOnePeer) {
  EXPECT_CALL(storage, getPeers())
      .WillOnce(Return(std::vector<model::Peer>(1)));
  EXPECT_CALL(provider, verify(A<const model::Block &>()))
      .WillOnce(Return(true));

  Block block;
  block.sigs.emplace_back();
  auto block_observable = rxcpp::observable<>::just(block);

  EXPECT_CALL(storage, apply(block, _)).WillOnce(Return(true));

  ASSERT_TRUE(validator.validateChain(block_observable, storage));
}