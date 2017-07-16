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

#ifndef IROHA_NETWORK_NODE_HPP
#define IROHA_NETWORK_NODE_HPP

#include <grpc++/grpc++.h>
#include <peer_service.grpc.pb.h>
#include <memory>
#include <uvw.hpp>
#include "node.hpp"

namespace peerservice {

  struct ConnectionTo : public::uvw::Emitter<ConnectionTo> {
    explicit ConnectionTo(const Node& n, std::shared_ptr<uvw::Loop> loop);

    ConnectionTo(const ConnectionTo&) = delete;
    ConnectionTo(const ConnectionTo&&) = delete;

    ~ConnectionTo();

    const Node &node;

    bool online;
    std::shared_ptr<uvw::TimerHandle> timer;

    void start_timer();
    void reset_timer();

    Heartbeat ping(Heartbeat* request);

   private:
    Heartbeat cachedHeartbeat;
    std::unique_ptr<PeerService::Stub> stub_;

    static std::default_random_engine generator;
    // from 1 sec to 2 secs
    static std::uniform_int_distribution<uint16_t> next_timer;
  };
}

#endif  // IROHA_NETWORK_NODE_HPP
