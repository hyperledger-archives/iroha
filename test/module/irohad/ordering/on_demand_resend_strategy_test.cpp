/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/ordering_gate_cache/on_demand_resend_strategy.hpp"

#include <gtest/gtest.h>
#include "module/shared_model/interface_mocks.hpp"
#include "ordering/impl/on_demand_common.hpp"

using namespace iroha::ordering;
using namespace iroha::consensus;

using ::testing::_;
using ::testing::Return;

/**
 * @given OnDemandResendStrategy instance
 * @when same batch is fed to the instance twice
 * @then first feeding succeeds, second one fails
 */
TEST(OnDemandResendStrategyTest, Feed) {
  OnDemandResendStrategy strategy;
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  ASSERT_TRUE(strategy.feed(batch));
  ASSERT_FALSE(strategy.feed(batch));
}

/**
 * @given OnDemandResendStrategy instance
 * @when readyToUse is called before and after batch is fed
 * @then first call fails, second succeeds
 */
TEST(OnDemandResendStrategyTest, ReadyToUse) {
  OnDemandResendStrategy strategy;
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  ASSERT_FALSE(strategy.readyToUse(batch));
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).WillOnce(Return(true));
  ASSERT_TRUE(strategy.readyToUse(batch));
}

/**
 * @given OnDemandResendStrategy instance
 * @when extract is called without any other prior calls
 * @then all possible future rounds are returned
 */
TEST(OnDemandResendStrategyTest, ExtractNonExisting) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  OnDemandResendStrategy::RoundSetType rounds{
      nextCommitRound(nextCommitRound(round)),
      nextCommitRound(nextRejectRound(round)),
      nextRejectRound(nextCommitRound(round)),
      nextRejectRound(nextRejectRound(round))};
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed and then extract are called for the same batch
 * @then all possible future rounds are returned
 */
TEST(OnDemandResendStrategyTest, ExtractNonReady) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  OnDemandResendStrategy::RoundSetType rounds{
      nextCommitRound(nextCommitRound(round)),
      nextCommitRound(nextRejectRound(round)),
      nextRejectRound(nextCommitRound(round)),
      nextRejectRound(nextRejectRound(round))};
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).WillOnce(Return(true));
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed, readyToUse are called, current round is set for 2 commits to
 * future and then extract is called for the same batch
 * @then all possible future rounds are returned
 */
TEST(OnDemandResendStrategyTest, ExtractCommitCommit) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).Times(2).WillRepeatedly(Return(true));
  strategy.readyToUse(batch);
  round = Round(3, 0);
  strategy.setCurrentRound(round);
  OnDemandResendStrategy::RoundSetType rounds{
      nextCommitRound(nextCommitRound(round)),
      nextCommitRound(nextRejectRound(round)),
      nextRejectRound(nextCommitRound(round)),
      nextRejectRound(nextRejectRound(round))};
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed, readyToUse are called, current round is set for (commit, reject)
 * to future and then extract is called for the same batch
 * @then all rounds except (reject, commit) are returned
 */
TEST(OnDemandResendStrategyTest, ExtractCommitReject) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).Times(2).WillRepeatedly(Return(true));
  strategy.readyToUse(batch);
  round = Round(2, 1);
  strategy.setCurrentRound(round);
  OnDemandResendStrategy::RoundSetType rounds{
      nextCommitRound(nextCommitRound(round)),
      nextRejectRound(nextCommitRound(round)),
      nextRejectRound(nextRejectRound(round))};
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed, readyToUse are called, current round is set for (reject, commit)
 * to future and then extract is called for the same batch
 * @then all rounds except (reject, commit) are returned
 */
TEST(OnDemandResendStrategyTest, ExtractRejectCommit) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).Times(2).WillRepeatedly(Return(true));
  strategy.readyToUse(batch);
  round = Round(2, 0);
  strategy.setCurrentRound(round);
  OnDemandResendStrategy::RoundSetType rounds{
      nextCommitRound(nextCommitRound(round)),
      nextRejectRound(nextCommitRound(round)),
      nextRejectRound(nextRejectRound(round))};
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed, readyToUse are called, current round is set for (reject, reject)
 * to future and then extract is called for the same batch
 * @then (reject, reject) are returned
 */
TEST(OnDemandResendStrategyTest, ExtractRejectReject) {
  Round round(1, 1);
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(round);
  shared_model::interface::types::HashType hash("hash");
  auto batch = createMockBatchWithHash(hash);
  strategy.feed(batch);
  EXPECT_CALL(*batch, Equals(_)).Times(2).WillRepeatedly(Return(true));
  strategy.readyToUse(batch);
  round = Round(1, 3);
  strategy.setCurrentRound(round);
  OnDemandResendStrategy::RoundSetType rounds{
      nextRejectRound(nextRejectRound(round))};
  ASSERT_EQ(strategy.extract(batch), rounds);
}

/**
 * @given OnDemandResendStrategy instance
 * @when feed, remove and feed are called for the same batch
 * @then both feed calls succeed
 */
TEST(OnDemandResendStrategyTest, Remove) {
  shared_model::interface::types::HashType hash("hash");
  auto tx = createMockTransactionWithHash(hash);
  auto batch = createMockBatchWithTransactions({tx}, "");
  OnDemandResendStrategy strategy;
  ASSERT_TRUE(strategy.feed(batch));
  strategy.remove({hash});
  ASSERT_TRUE(strategy.feed(batch));
}

/**
 * @given OnDemandResendStrategy instance
 * @when setCurrentRound for (1, 1) is called
 * @then getCurrentRound returns (1, 1)
 */
TEST(OnDemandResendStrategyTest, SetGetRound) {
  OnDemandResendStrategy strategy;
  strategy.setCurrentRound(Round(1, 1));
  ASSERT_EQ(strategy.getCurrentRound(), Round(1, 1));
}
