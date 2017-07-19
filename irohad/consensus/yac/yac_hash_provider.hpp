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

#ifndef IROHA_YAC_HASH_PROVIDER_HPP
#define IROHA_YAC_HASH_PROVIDER_HPP

#include <vector>
#include <string>
#include "model/block.hpp"
#include "model/peer.hpp"
#include "consensus/yac/cluster_order.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      class YacHash {
       public:
        std::string hash_value;
      };

      /**
       * Provide methods related to hash operations in ya consensus
       */
      class YacHashProvider {

        /**
         * Make hash from block
         * @param block - for hashing
         * @return hashed value of block
         */
        virtual YacHash makeHash(model::Block block) = 0;

        /**
         * Make ordering on cluster
         * @param hash
         * @param initial_order
         * @return cluster ordering based on hash and initial order
         */
        virtual ClusterOrdering order(YacHash hash,
                                      std::vector<model::Peer> initial_order) = 0;

        virtual ~YacHashProvider() = default;
      };
    } // namespace yac
  } // namespace consensus
} // iroha

#endif //IROHA_YAC_HASH_PROVIDER_HPP
