/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "multi_sig_transactions/mst_notifier_impl.hpp"

#include "framework/specified_visitor.hpp"
#include "module/irohad/multi_sig_transactions/mst_mocks.hpp"
#include "module/irohad/multi_sig_transactions/mst_test_helpers.hpp"
#include "module/irohad/network/network_mocks.hpp"
#include "module/irohad/torii/torii_mocks.hpp"
#include "module/shared_model/builders/protobuf/test_transaction_builder.hpp"
#include "module/shared_model/tx_response_status_mocks.hpp"

using ::testing::_;
using ::testing::Return;

class MstNotifierTest : public ::testing::Test {
 public:
  void SetUp() override {
    pcs = std::make_shared<iroha::network::MockPeerCommunicationService>();

    status_factory = std::make_shared<TxStatusFactoryMock>();

    status_bus = std::make_shared<iroha::torii::MockStatusBus>();

    mst_processor = std::make_shared<iroha::MockMstProcessor>();
    EXPECT_CALL(*mst_processor, onStateUpdateImpl())
        .WillRepeatedly(Return(update_state_subject.get_observable()));
    EXPECT_CALL(*mst_processor, onPreparedBatchesImpl())
        .WillRepeatedly(Return(prepared_batch_subject.get_observable()));
    EXPECT_CALL(*mst_processor, onExpiredBatchesImpl())
        .WillRepeatedly(Return(expired_batch_subject.get_observable()));

    mst_notifier = std::make_shared<iroha::MstNotifierImpl>(
        mst_processor, pcs, status_bus, status_factory);
  }

  /// creates mst state with two batches
  auto createMstState() {
    auto mst_state = iroha::MstState::empty();

    auto time = iroha::time::now();
    mst_state += addSignatures(
        makeTestBatch(txBuilder(1, time)), 0, makeSignature("1", "pub_key_1"));
    mst_state += addSignatures(makeTestBatch(txBuilder(1, time + 1)),
                               0,
                               makeSignature("1", "pub_key_1"));
    return std::make_shared<iroha::MstState>(mst_state);
  }

  /// creates batch with one transaction inside
  auto createBatch() {
    auto time = iroha::time::now();
    return addSignatures(
        makeTestBatch(txBuilder(1, time)), 0, makeSignature("1", "pub_key_1"));
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

  std::shared_ptr<iroha::network::MockPeerCommunicationService> pcs;
  std::shared_ptr<TxStatusFactoryMock> status_factory;
  std::shared_ptr<iroha::torii::MockStatusBus> status_bus;
  std::shared_ptr<iroha::MockMstProcessor> mst_processor;

  // tested component
  std::shared_ptr<iroha::MstNotifier> mst_notifier;

  StatusMapType status_map;

  // input from MST
  rxcpp::subjects::subject<iroha::MstProcessor::UpdatedStateType>
      update_state_subject;
  rxcpp::subjects::subject<iroha::MstProcessor::BatchType>
      prepared_batch_subject;
  rxcpp::subjects::subject<iroha::MstProcessor::BatchType>
      expired_batch_subject;
};

/**
 * @given Mst notifier
 * @when  Mst processor shares updated batches
 * @then  Mst notifier should emit MstPending statuses for corresponding
 * transactions
 */
TEST_F(MstNotifierTest, OnUpdateNotify) {
  auto mst_state = createMstState();
  auto invoke_times = mst_state->getBatches().size();
  EXPECT_CALL(*pcs, propagate_batch(_)).Times(0);
  // here we don't care about return value of factory because it relays to
  // status_bus mock
  EXPECT_CALL(*status_factory, makeMstPending(_, _)).Times(invoke_times);
  EXPECT_CALL(*status_bus, publish(_)).Times(invoke_times);

  update_state_subject.get_subscriber().on_next(mst_state);
}

/**
 * @given Mst notifier
 * @when  Mst processor shares expired batches
 * @then all transactions in batches have Expired statuses for corresponding
 * transactions
 */
TEST_F(MstNotifierTest, OnExpiredNotify) {
  auto batch = createBatch();
  auto invoke_times = batch->transactions().size();
  EXPECT_CALL(*pcs, propagate_batch(_)).Times(0);
  // here we don't care about return value of factory because it relays to
  // status_bus mock
  EXPECT_CALL(*status_factory, makeMstExpired(_, _)).Times(invoke_times);
  EXPECT_CALL(*status_bus, publish(_)).Times(invoke_times);

  expired_batch_subject.get_subscriber().on_next(batch);
}

/**
 * @given Mst notifier
 * @when  Mst processor shares complited batches
 * @then  Mst notifier invoke propagation of batch on PCS
 *        @and set EnoughSignatures statues for corresponding transactions
 */
TEST_F(MstNotifierTest, OnCompletedNotify) {
  auto batch = createBatch();
  auto invoke_times = batch->transactions().size();
  EXPECT_CALL(*pcs, propagate_batch(_)).Times(invoke_times);
  // here we don't care about return value of factory because it relays to
  // status_bus mock
  EXPECT_CALL(*status_factory, makeEnoughSignaturesCollected(_, _))
      .Times(invoke_times);
  EXPECT_CALL(*status_bus, publish(_)).Times(invoke_times);

  prepared_batch_subject.get_subscriber().on_next(batch);
}
