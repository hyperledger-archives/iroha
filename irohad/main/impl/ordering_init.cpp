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

#include "main/impl/ordering_init.hpp"

namespace iroha {
  namespace network {
    auto OrderingInit::createGate(std::string network_address) {
      return std::make_shared<ordering::OrderingGateImpl>(network_address);
    }

    auto OrderingInit::createService(std::vector<model::Peer> peers,
                                     size_t max_size,
                                     size_t delay_milliseconds,
                                     std::shared_ptr<uvw::Loop> loop) {

      return std::make_shared<ordering::OrderingServiceImpl>(peers,
                                                             max_size,
                                                             delay_milliseconds,
                                                             loop);
    }

    std::shared_ptr<ordering::OrderingGateImpl> OrderingInit::initOrderingGate(
        std::vector<model::Peer> peers, std::shared_ptr<uvw::Loop> loop,
        size_t max_size, size_t delay_milliseconds) {
      ordering_service =
          createService(peers, max_size, delay_milliseconds, loop);
      ordering_gate = createGate(peers.front().address);
      return ordering_gate;
    }
  }  // namespace network
}  // namespace iroha