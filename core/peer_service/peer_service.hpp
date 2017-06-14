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
      std::string _ip;
      std::string _public_key;
      std::string _name;
      double _trust;
      State _state;

      Node(std::string ip = defaultIP(),
           std::string public_key = defaultPubKey(),
           std::string name = defaultName(),
           double trust = 100.0,
           State state = PREPARE)
          : _ip(ip),
            _public_key(public_key),
            _name(name),
            _trust(trust),
            _state(state) {}

      Node(const Node &p) :
        _ip(p._ip),
        _public_key(p._public_key),
        _name(p._name),
        _trust(p._trust),
        _state(p._state) {}


      bool isDefaultIP() const { return _ip == defaultIP(); }
      bool isDefaultPubKey() const { return _public_key == defaultPubKey(); }
    };

    using Nodes = std::vector<std::shared_ptr<Node>>;


    Nodes _peer_list;
    Nodes _active_peer_list;

}  // namespace peer_service

#endif
