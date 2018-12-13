/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "consensus/yac/supermajority_checker.hpp"
#include "consensus/yac/impl/supermajority_checker_bft.hpp"
#include "consensus/yac/impl/supermajority_checker_cft.hpp"

#include <gtest/gtest.h>
#include <boost/foreach.hpp>
#include "boost/tuple/tuple.hpp"
#include "consensus/yac/supermajority_checker.hpp"
#include "logger/logger.hpp"
#include "module/shared_model/interface_mocks.hpp"

using namespace iroha::consensus::yac;

using ::testing::ReturnRefOfCopy;

static logger::Logger log_ = logger::testLog("YacCommon");

static const std::map<ConsistencyModel, unsigned int> kf1_param{
    {ConsistencyModel::kCft, detail::kSupermajorityCheckerKfPlus1Cft},
    {ConsistencyModel::kBft, detail::kSupermajorityCheckerKfPlus1Bft}};

class SupermajorityCheckerTest
    : public ::testing::TestWithParam<ConsistencyModel>,
      public SupermajorityChecker {
 public:
  void SetUp() override {}

  unsigned int getK() const {
    return kf1_param.at(GetParam());
  }

  size_t getAllowedFaultyPeers(size_t total_peers) const {
    return (total_peers - 1) / getK();
  }

  size_t getSupermajority(size_t total_peers) const {
    return total_peers - getAllowedFaultyPeers(total_peers);
  }

  std::string modelToString() const {
    return "`" + std::to_string(getK()) + " * f + 1' "
        + (GetParam() == ConsistencyModel::kBft ? "BFT" : "CFT") + " model";
  }

  bool hasSupermajority(PeersNumberType current,
                        PeersNumberType all) const override {
    return checker->hasSupermajority(current, all);
  }

  bool canHaveSupermajority(const VoteGroups &votes,
                            PeersNumberType all) const override {
    return checker->canHaveSupermajority(votes, all);
  }

  std::unique_ptr<SupermajorityChecker> checker{
      getSupermajorityChecker(GetParam())};
};

using CftAndBftSupermajorityCheckerTest = SupermajorityCheckerTest;
using BftSupermajorityCheckerTest = SupermajorityCheckerTest;

INSTANTIATE_TEST_CASE_P(Instance,
                        CftAndBftSupermajorityCheckerTest,
                        ::testing::Values(ConsistencyModel::kCft,
                                          ConsistencyModel::kBft));

INSTANTIATE_TEST_CASE_P(Instance,
                        BftSupermajorityCheckerTest,
                        ::testing::Values(ConsistencyModel::kBft));

/**
 * @given 2 participants
 * @when check range of voted participants
 * @then correct result
 */
TEST_P(CftAndBftSupermajorityCheckerTest, SuperMajorityCheckWithSize2) {
  log_->info("-----------| F(x, 2), x in [0..3] |-----------");

  size_t A = 2; // number of all peers
  for (size_t i = 0; i < 4; ++i) {
    if (i >= getSupermajority(A)  // enough votes
        and i <= A                // not more than total peers amount
    ) {
      ASSERT_TRUE(hasSupermajority(i, A))
          << i << " votes out of " << A << " are supermajority in "
          << modelToString();
    } else {
      ASSERT_FALSE(hasSupermajority(i, A))
          << i << " votes out of " << A << " are not a supermajority in "
          << modelToString();
    }
  }
}

/**
 * @given 6 participants
 * @when check range of voted participants
 * @then correct result
 */
TEST_P(CftAndBftSupermajorityCheckerTest, SuperMajorityCheckWithSize4) {
  log_->info("-----------| F(x, 6), x in [0..7] |-----------");

  size_t A = 6; // number of all peers
  for (size_t i = 0; i < 8; ++i) {
    if (i >= getSupermajority(A)  // enough votes
        and i <= A                // not more than total peers amount
    ) {
      ASSERT_TRUE(hasSupermajority(i, A))
          << i << " votes out of " << A << " are supermajority in "
          << modelToString();
    } else {
      ASSERT_FALSE(hasSupermajority(i, A))
          << i << " votes out of " << A << " are not a supermajority in "
          << modelToString();
    }
  }
}

/**
 * @given some peers vote all for one option, others vote each for own option,
 * and the rest do not vote
 * @when different amounts of described peer kinds
 * @then correct decision on supermajority possibility
 */
TEST_P(CftAndBftSupermajorityCheckerTest, OneLargeAndManySingleVoteGroups) {
  /**
   * Make vote groups out of given votes amount such that the largest one has
   * the given amount of votes.
   */
  auto makeVoteGroups = [](size_t largest_group, size_t voted_peers) {
    BOOST_ASSERT_MSG(
        largest_group <= voted_peers,
        "A votes group cannot have more votes than the amount of voted peers!");
    std::vector<size_t> vote_groups;
    const size_t num_groups = voted_peers - largest_group + 1;
    vote_groups.reserve(num_groups);
    std::fill_n(std::back_inserter(vote_groups), num_groups, 1);
    vote_groups[0] = largest_group;
    return vote_groups;
  };

  struct Case {
    size_t V; // Voted peers
    size_t A; // All peers
  };

  for (const auto &c : std::initializer_list<Case>({{6, 7}, {8, 12}})) {
    log_->info("--------| ProofOfReject(x, {0}, {1}), x in [1..{0}] |---------",
               c.V,
               c.A);

    size_t L;  // number of votes for the Leading option
    for (L = 0; L <= c.V; ++L) {
      const auto vote_groups = makeVoteGroups(L, c.V);
      const size_t N = c.A - c.V;              // not yet voted
      const size_t S = getSupermajority(c.A);  // supermajority
      const size_t M =  // amount of malicious peers that may have sent their
                        // votes for the L group to other peers (only in BFT)
          (GetParam() == ConsistencyModel::kBft
               ? std::min(c.A - N - L, getAllowedFaultyPeers(c.A))
               : 0);
      const size_t Lp =  // max possible amount for the L group on other peers
          L              // the votes we know
          + N            // the peers that we have no votes from
          + M;           // some malicious peers that may have voted twice
      // Check if any peer on the network can get supermajority:
      if (Lp >= S) {
        EXPECT_TRUE(canHaveSupermajority(vote_groups, c.A))
            << "if " << N << " not yet voted peers and " << M
            << " malicious double voting peers vote for option already having "
            << L << " votes, it will reach supermajority in "
            << modelToString();
      } else {
        EXPECT_FALSE(canHaveSupermajority(vote_groups, c.A))
            << "even if all " << N << " not yet voted peers and " << M
            << " malicious double voting peers vote for option already having "
            << L << " votes, it will not reach supermajority in "
            << modelToString();
      }
    }
  }
}

/**
 * @given some honest peers vote for one of two options, the rest honest peers
 * have not voted yet, malicious peers vote for the first option and then for
 * the second
 * @when different amounts of described peer kinds
 * @then correct decision on supermajority possibility
 */
TEST_P(BftSupermajorityCheckerTest, ProofOfRejectWithMaliciousVotingTwice) {
  struct Case {
    const size_t V1;  // Votes for option 1.
    const size_t V2;  // Votes for option 2.
    const size_t N;   // Not voted peers.
  };

  for (const auto &c : std::initializer_list<Case>({{6, 7, 8}})) {
    const size_t A = c.V1 + c.V2 + c.N;  // All peers amount.
    const bool supermajority_checker_result =
        canHaveSupermajority(std::vector<size_t>{c.V1, c.V2}, A);

    log_->info(
        "-----------| RejectProof([{}, {}], {}) |-----------", c.V1, c.V2, A);

    // suppose all the malicious peers voted for one of the options
    // (let it be `a') also voted for the other option (let it be `b').
    // Check if the option `a' will have supermajority if the not yet voted
    // peers support the option `a'.
    const auto check =
        [&](size_t Va,  // the number of votes for option `a' after the
                        // malicious peers double voted
            size_t Vb   // the number of votes in the option `b', for which the
                        // double voted malicious peers voted initially
        ) {
          const size_t Mp = getAllowedFaultyPeers(A);  // the possible Malicious
                                                       // peers amount
          const size_t Mb = std::min(Mp, Vb);    // the possible Malicious peers
                                                 // voted for option `b' amount
          const size_t S = getSupermajority(A);  // Supermajority.

          EXPECT_EQ(Va + c.N + Mb >= S, supermajority_checker_result)
              << "In the case of " << Va
              << " peers initially voted for option `a', " << Vb
              << " peers for option `b', and " << Mb
              << " out of the ones voted for `b' are malicious and vote also "
                 "for `a', then `a' will "
              << (Va + c.N + Mb >= S ? "" : "not ") << "have supermajority of "
              << S << " after " << c.N
              << " not yet voted peers also choose `a' in " << modelToString();
        };

    check(c.V1, c.V2);
    check(c.V2, c.V1);
  }
}
