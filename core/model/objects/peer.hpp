/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp

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

#ifndef IROHA_PEER_HPP
#define IROHA_PEER_HPP

#include <string>

namespace object {

    struct Peer{

        std::string publicKey;
        std::string address;

        explicit Peer(const Peer* a):
            publicKey(a->publicKey),
            address(a->address)
        {}

        explicit Peer(
            std::string publicKey,
            std::string address
        ):
            publicKey(std::move(publicKey)),
            address(std::move(address))
        {}

        explicit Peer(
            std::string publicKey
        ):
            publicKey(std::move(publicKey))
        {}

    };

};  // namespace object

#endif //IROHA_PEER_HPP
