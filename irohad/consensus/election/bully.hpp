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

#ifndef IROHA_BULLY_CLIENT_HPP
#define IROHA_BULLY_CLIENT_HPP

#include <election.grpc.pb.h>
#include <grpc++/create_channel.h>
#include <messages.hpp>
#include <peer_service/node.hpp>
#include <uvw/emitter.hpp>

namespace consensus {
  namespace election {

    using Node = peerservice::Node;
    using pubkey_t = iroha::ed25519::pubkey_t;

    class Bully : public uvw::Emitter<Bully> {
     public:
      Bully(const std::vector<Node> &cluster, const pubkey_t &self);

     private:
      const std::vector<Node> &cluster_;
      const pubkey_t self_;

      struct Client : public Node {
        void create_channel() {
          auto to = this->ip + ":" + std::to_string(this->port);
          auto channel =
              grpc::CreateChannel(to, grpc::InsecureChannelCredentials());
          stub_ = ElectionService::NewStub(channel);
        }

        std::unique_ptr<ElectionService::Stub> stub_;
      };
    };
  }
}

#endif  // IROHA_BULLY_CLIENT_HPP
