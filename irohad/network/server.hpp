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

#ifndef IROHA_NETWORK_HPP
#define IROHA_NETWORK_HPP

#include <grpc++/server_builder.h>
#include <consensus/consensus_service.hpp>
#include <consensus/messages.hpp>
#include <uvw.hpp>

namespace network {

  /**
   * This is an entity, which is listens host:port and emits received messages
   * It emits all types of messages defined in consensus/messages.hpp
   * GRPC server builder is also here.
   */
  class Server : public uvw::Emitter<Server> {
   public:
    Server(std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault());

    void run_grpc(uvw::Addr addr, bool blocking = true);

   private:
    std::shared_ptr<uvw::Loop> loop_;

    grpc::ServerBuilder builder;
    consensus::ConsensusService consensus_;
  };
}
#endif  // IROHA_NETWORK_HPP
