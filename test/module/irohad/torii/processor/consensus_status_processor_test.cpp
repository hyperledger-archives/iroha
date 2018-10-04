/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/range/join.hpp>

#include "builders/protobuf/transaction.hpp"
#include "interfaces/iroha_internal/transaction_batch.hpp"
#include "interfaces/iroha_internal/transaction_sequence_factory.hpp"
#include "torii/impl/status_bus_impl.hpp"
#include "torii/processor/consensus_status_processor_impl.hpp"

#include "framework/batch_helper.hpp"
#include "framework/specified_visitor.hpp"
#include "framework/test_subscriber.hpp"
#include "module/irohad/network/network_mocks.hpp"
#include "module/irohad/torii/status_mocks.hpp"
#include "module/irohad/torii/torii_mocks.hpp"
#include "module/shared_model/builders/protobuf/common_objects/proto_signature_builder.hpp"
#include "module/shared_model/builders/protobuf/proposal.hpp"
#include "module/shared_model/builders/protobuf/test_block_builder.hpp"
#include "module/shared_model/builders/protobuf/test_proposal_builder.hpp"
#include "module/shared_model/builders/protobuf/test_transaction_builder.hpp"

using namespace iroha;
using namespace iroha::network;
using namespace iroha::torii;
using namespace iroha::synchronizer;
using namespace framework::test_subscriber;

using ::testing::_;
using ::testing::A;
using ::testing::Return;

using shared_model::interface::TransactionBatch;

class ConsensusStatusProcessorTest : public ::testing::Test {
 public:
  void SetUp() override {
    pcs = std::make_shared<MockPeerCommunicationService>();
    status_factory = std::make_shared<TxStatusFactoryMock>();

    EXPECT_CALL(*pcs, on_commit())
        .WillRepeatedly(Return(commit_notifier.get_observable()));
    EXPECT_CALL(*pcs, on_verified_proposal())
        .WillRepeatedly(Return(verified_prop_notifier.get_observable()));

    status_bus = std::make_shared<MockStatusBus>();
    csp = std::make_shared<ConsensusStatusProcessorImpl>(
        pcs, status_bus, status_factory);
  }

  auto base_tx() {
    return shared_model::proto::TransactionBuilder()
        .creatorAccountId("user@domain")
        .createdTime(iroha::time::now())
        .setAccountQuorum("user@domain", 2)
        .quorum(1);
  }

  auto baseTestTx(shared_model::interface::types::QuorumType quorum = 1) {
    return TestTransactionBuilder()
        .createdTime(iroha::time::now())
        .creatorAccountId("user@domain")
        .setAccountQuorum("user@domain", 2)
        .quorum(quorum)
        .build();
  }

  inline auto makeKey() {
    return shared_model::crypto::DefaultCryptoAlgorithmType::generateKeypair();
  }

  template <typename Transaction, typename... KeyPairs>
  auto addSignaturesFromKeyPairs(Transaction &&tx, KeyPairs... keypairs) {
    auto create_signature = [&](auto &&key_pair) {
      auto &payload = tx.payload();
      auto signedBlob = shared_model::crypto::CryptoSigner<>::sign(
          shared_model::crypto::Blob(payload), key_pair);
      tx.addSignature(signedBlob, key_pair.publicKey());
    };

    int temp[] = {(create_signature(std::forward<KeyPairs>(keypairs)), 0)...};
    (void)temp;

    return tx;
  }

 protected:
  using StatusMapType = std::unordered_map<
      shared_model::crypto::Hash,
      std::shared_ptr<shared_model::interface::TransactionResponse>,
      shared_model::crypto::Hash::Hasher>;

  /**
   * Checks if all transactions have corresponding status
   * @param transactions transactions to check status
   * @param status to be checked
   */
  template <typename Status>
  void validateStatuses(
      std::vector<shared_model::proto::Transaction> &transactions) {
    for (const auto &tx : transactions) {
      auto tx_status = status_map.find(tx.hash());
      ASSERT_NE(tx_status, status_map.end());
      ASSERT_NO_THROW(boost::apply_visitor(
          framework::SpecifiedVisitor<Status>(), tx_status->second->get()));
    }
  }

  std::shared_ptr<MockPeerCommunicationService> pcs;
  std::shared_ptr<MockStatusBus> status_bus;
  std::shared_ptr<TxStatusFactoryMock> status_factory;

  std::shared_ptr<ConsensusStatusProcessorImpl> csp;

  StatusMapType status_map;
  shared_model::builder::TransactionStatusBuilder<
      shared_model::proto::TransactionStatusBuilder>
      status_builder;

  rxcpp::subjects::subject<SynchronizationEvent> commit_notifier;
  rxcpp::subjects::subject<
      std::shared_ptr<iroha::validation::VerifiedProposalAndErrors>>
      verified_prop_notifier;

  const size_t proposal_size = 5;
  const size_t block_size = 3;
};

/**
 * @given cs processor
 * @when transactions composed the block
 * @then for every transaction in the bathces STATEFUL_VALID status is returned
 */
TEST_F(ConsensusStatusProcessorTest, TransactionProcessorBlockCreatedTest) {
  std::vector<shared_model::proto::Transaction> txs;
  for (size_t i = 0; i < proposal_size; i++) {
    auto &&tx = addSignaturesFromKeyPairs(baseTestTx(), makeKey());
    txs.push_back(tx);
  }

  EXPECT_CALL(*status_bus, publish(_)).Times(txs.size());

  EXPECT_CALL(*status_factory, makeStatefulValid(_, _)).Times(txs.size());

  // 1. Create proposal and notify transaction processor about it
  auto proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder().transactions(txs).build());

  // empty transactions errors - all txs are valid
  verified_prop_notifier.get_subscriber().on_next(
      std::make_shared<iroha::validation::VerifiedProposalAndErrors>(
          std::make_pair(proposal, iroha::validation::TransactionsErrors{})));

  auto block = TestBlockBuilder().transactions(txs).build();

  // 2. Create block and notify transaction processor about it
  rxcpp::subjects::subject<std::shared_ptr<shared_model::interface::Block>>
      blocks_notifier;

  commit_notifier.get_subscriber().on_next(SynchronizationEvent{
      blocks_notifier.get_observable(), SynchronizationOutcomeType::kCommit});

  blocks_notifier.get_subscriber().on_next(
      std::shared_ptr<shared_model::interface::Block>(clone(block)));
  // Note blocks_notifier hasn't invoked on_completed, so
  // transactions are not commited
}

/**
 * @given cs processor
 * @when  transactions compose proposal which is sent to peer
 * communication service
 * @and   all transactions composed the block @and were committed
 * @then for every transaction COMMIT status is returned
 */
TEST_F(ConsensusStatusProcessorTest, TransactionProcessorOnCommitTest) {
  std::vector<shared_model::proto::Transaction> txs;
  for (size_t i = 0; i < proposal_size; i++) {
    auto &&tx = addSignaturesFromKeyPairs(baseTestTx(), makeKey());
    txs.push_back(tx);
  }

  EXPECT_CALL(*status_bus, publish(_)).Times(txs.size() * 2);

  EXPECT_CALL(*status_factory, makeStatefulValid(_, _)).Times(txs.size());
  EXPECT_CALL(*status_factory, makeCommitted(_, _)).Times(txs.size());

  // 1. Create proposal and notify transaction processor about it
  auto proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder().transactions(txs).build());

  // empty transactions errors - all txs are valid
  verified_prop_notifier.get_subscriber().on_next(
      std::make_shared<iroha::validation::VerifiedProposalAndErrors>(
          std::make_pair(proposal, iroha::validation::TransactionsErrors{})));

  auto block = TestBlockBuilder().transactions(txs).build();

  // 2. Create block and notify transaction processor about it
  SynchronizationEvent commit_event{
      rxcpp::observable<>::just(
          std::shared_ptr<shared_model::interface::Block>(clone(block))),
      SynchronizationOutcomeType::kCommit};
  commit_notifier.get_subscriber().on_next(commit_event);
}

/**
 * @given cs processor
 * @when  some transactions became part of block, while some
 * were not committed, failing stateful validation
 * @then  for every transaction from block COMMIT status is returned
 * @and   for every transaction, which failed stateful validation,
 * STATEFUL_INVALID_STATUS status is returned
 */
TEST_F(ConsensusStatusProcessorTest, TransactionProcessorInvalidTxsTest) {
  std::vector<shared_model::proto::Transaction> block_txs;
  for (size_t i = 0; i < block_size; i++) {
    auto &&tx = TestTransactionBuilder().createdTime(i).build();
    block_txs.push_back(tx);
    status_map[tx.hash()] =
        status_builder.notReceived().txHash(tx.hash()).build();
  }

  std::vector<shared_model::proto::Transaction> invalid_txs;

  for (size_t i = block_size; i < proposal_size; i++) {
    auto &&tx = TestTransactionBuilder().createdTime(i).build();
    invalid_txs.push_back(tx);
    status_map[tx.hash()] =
        status_builder.notReceived().txHash(tx.hash()).build();
  }

  // For all transactions from proposal
  // transaction will be published twice
  // (first that they are stateless
  // valid and second that they either
  // passed or not stateful validation)
  // Plus all transactions from block will
  // be committed and corresponding status will be sent
  EXPECT_CALL(*status_bus, publish(_)).Times(proposal_size + block_size);

  EXPECT_CALL(*status_factory, makeStatefulFail(_, _))
      .Times(invalid_txs.size());
  EXPECT_CALL(*status_factory, makeStatefulValid(_, _))
      .Times(proposal_size - invalid_txs.size());
  EXPECT_CALL(*status_factory, makeCommitted(_, _))
      .Times(proposal_size - invalid_txs.size());

  auto proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder()
          .transactions(boost::join(block_txs, invalid_txs))
          .build());

  // trigger the verified event with txs, which we want to fail, as errors
  auto verified_proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder().transactions(block_txs).build());
  auto txs_errors = iroha::validation::TransactionsErrors{};
  for (size_t i = 0; i < invalid_txs.size(); ++i) {
    txs_errors.push_back(std::make_pair(
        iroha::validation::CommandError{
            "SomeCommandName", "SomeCommandError", true, i},
        invalid_txs[i].hash()));
  }
  verified_prop_notifier.get_subscriber().on_next(
      std::make_shared<iroha::validation::VerifiedProposalAndErrors>(
          std::make_pair(verified_proposal, txs_errors)));

  auto block = TestBlockBuilder().transactions(block_txs).build();

  SynchronizationEvent commit_event{
      rxcpp::observable<>::just(
          std::shared_ptr<shared_model::interface::Block>(clone(block))),
      SynchronizationOutcomeType::kCommit};
  commit_notifier.get_subscriber().on_next(commit_event);
}
