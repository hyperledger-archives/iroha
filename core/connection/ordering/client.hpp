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

#ifndef __IROHA_CONNECTION_ORDERING_CLIENT_HPP__
#define __IROHA_CONNECTION_ORDERING_CLIENT_HPP__

#include <block.pb.h>

namespace connection {
    namespace ordering {

        bool send(std::string ip, iroha::protocol::Transaction& tx);
        class OrderingService {
        public:
            iroha::protocol::QueueTransactionResponse* QueueTransaction(
                const iroha::protocol::Transaction& tx);
        };

    }  // namespace consensus
}  // namespace connection

#endif // __IROHA_CONNECTION_ORDERING_CLIENT_HPP__