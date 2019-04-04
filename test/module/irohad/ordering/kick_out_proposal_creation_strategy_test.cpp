/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/kick_out_proposal_creation_strategy.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "module/irohad/consensus/yac/mock_yac_supermajority_checker.hpp"
#include "module/irohad/consensus/yac/yac_test_util.hpp"

using namespace iroha::ordering;

using testing::_;
using testing::Return;

class KickOutProposalCreationStrategyTest : public testing::Test {
 public:
  void SetUp() override {
    for (auto i = 0u; i < number_of_peers; ++i) {
      peers.push_back(iroha::consensus::yac::makePeer(std::to_string(i)));
    }

    supermajority_checker_ =
        std::make_shared<iroha::consensus::yac::MockSupermajorityChecker>();
    strategy_ = std::make_shared<KickOutProposalCreationStrategy>(
        supermajority_checker_);
  }

  std::shared_ptr<KickOutProposalCreationStrategy> strategy_;
  std::shared_ptr<iroha::consensus::yac::MockSupermajorityChecker>
      supermajority_checker_;

  KickOutProposalCreationStrategy::PeerList peers;
  size_t number_of_peers = 7;
  size_t f = 2;
};

/**
 * @given initialized kickOutStrategy
 *        @and onCollaborationOutcome is invoked for the first round
 * @when  onProposal calls F times with different peers for further rounds
 * @then  onCollaborationOutcome call returns true
 */
TEST_F(KickOutProposalCreationStrategyTest, OnNonMaliciousCase) {
  EXPECT_CALL(*supermajority_checker_, hasMajority(0, 0))
      .WillOnce(Return(false));

  ASSERT_EQ(false, strategy_->onCollaborationOutcome({1, 0}, peers));

  for (auto i = 0u; i < f; ++i) {
    strategy_->onProposal(peers.at(i), {2, 0});
  }

  EXPECT_CALL(*supermajority_checker_, hasMajority(f, number_of_peers))
          .WillOnce(Return(true));
  ASSERT_EQ(true, strategy_->onCollaborationOutcome({2, 0}, peers));
}
