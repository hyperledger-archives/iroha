/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#ifndef __IROHA_PEER_SERVICE_PEER_SERVIEC_HPP__
#define __IROHA_PEER_SERVICE_PEER_SERVIEC_HPP__

#include <memory>
#include <string>
#include <vector>

namespace peer_service {

    enum State { PREPARE, READY, ACTIVE };

    inline static const std::string defaultName() { return ""; }

    inline static const std::string defaultIP() { return ""; }

    inline static const std::string defaultPubKey() { return ""; }

    struct Node {
      std::string ip_;
      std::string public_key_;
      std::string name_;
      double trust_;
      State state_;

      Node(std::string ip = defaultIP(),
           std::string public_key = defaultPubKey(),
           std::string name = defaultName(),
           double trust = 100.0,
           State state = PREPARE)
          : ip_(ip),
            public_key_(public_key),
            name_(name),
            trust_(trust),
            state_(state) {}

      Node(const Node &p) :
        ip_(p.ip_),
        public_key_(p.public_key_),
        name_(p.name_),
        trust_(p.trust_),
        state_(p.state_) {}


      bool isDefaultIP() const { return ip_ == defaultIP(); }
      bool isDefaultPubKey() const { return public_key_ == defaultPubKey(); }
    };

    using Nodes = std::vector<std::shared_ptr<Node>>;


    Nodes peer_list_;
    Nodes active_peer_list_;

}  // namespace peer_service

#endif
