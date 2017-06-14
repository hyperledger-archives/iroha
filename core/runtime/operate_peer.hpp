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
#ifndef IROHA_RUNTIME_OPERATE_PEER_HPP
#define IROHA_RUNTIME_OPERATE_PEER_HPP

#include <block.pb.h>
#include "base.hpp"
#include <peer_service/change_state.hpp>

namespace runtime {
    using Transaction = iroha::protocol::Transaction;

    struct OperatePeer : public base {
        void processTransaction(const Transaction& tx){
            // ToDo add some command
            // peer_service::change_state::runtime::add();
        }
    };
}

#endif //IROHA_RUNTIME_OPERATE_PEER_HPP
