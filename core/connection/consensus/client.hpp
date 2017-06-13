/*
Copyright 2017 Soramitsu Co., Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CONNECTION_CONSENSUS_CLIENT_HPP
#define CONNECTION_CONSENSUS_CLIENT_HPP

#include <block.pb.h>

namespace connection {
    namespace consensus {

//        using PeerNodesIterator = std::vector<peer_service::self_state::Nodes>::const_iterator;
        using PeerNodesIterator = std::vector<int>::const_iterator;

        class SumeragiClient {
        public:
            bool unicast(const iroha::protocol::Block& block);
            bool multicast(const iroha::protocol::Block& block,
                           PeerNodesIterator begin, PeerNodesIterator end);
            bool commit(const iroha::protocol::Block& block);
        };

    }  // namespace consensus
}  // namespace connection

#endif
