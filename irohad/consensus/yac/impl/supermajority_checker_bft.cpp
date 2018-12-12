/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "consensus/yac/impl/supermajority_checker_bft.hpp"

#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/numeric.hpp>
#include "consensus/yac/impl/supermajority_checker_kf1.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      bool SupermajorityCheckerBft::hasSupermajority(
          PeersNumberType agreed, PeersNumberType all) const {
        return checkKfPlus1Supermajority(
            agreed, all, detail::kSupermajorityCheckerKfPlus1Bft);
      }

      bool SupermajorityCheckerBft::canHaveSupermajority(
          const VoteGroups &votes, PeersNumberType all) const {
        const PeersNumberType largest_group = *boost::max_element(votes);
        const PeersNumberType voted = boost::accumulate(votes, 0);
        const PeersNumberType not_voted = all - voted;
        const PeersNumberType possible_malicious_switch_to_frequent =
            std::min((all - 1) / 5,         // tolerated malicious peers
                     voted - largest_group  // that voted for other options
            );

        return hasSupermajority(
            std::min(largest_group + not_voted
                         + possible_malicious_switch_to_frequent,
                     all),
            all);
      }

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha
