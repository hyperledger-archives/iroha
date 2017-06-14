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
#ifndef __IROHA_PEER_SERVICE_SELF_STATE_HPP__
#define __IROHA_PEER_SERVICE_SELF_STATE_HPP__

#include <string>

namespace peer_service{
    enum State{
        PREPARE, READY, ACTIVE
    };
    namespace self_state{

        std::string getPublicKey();
        std::string getPrivateKey();
        std::string getIp();

        bool  isLeader();
        State state();

        void activate();
        void stop();

    };
};

#endif //__IROHA_PEER_SERVICE_SELF_STATE_HPP__
