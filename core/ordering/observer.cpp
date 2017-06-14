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
#include "observer.hpp"
#include "quque.hpp"
#include <common/timer.hpp>
//#include <peer_service/self_state.hpp>

namespace ordering {
    namespace observer {
        // This is invoked in thread.
        void observe() {
            while (1) {
                timer::setAwkTimer(5000, []() {
                    if( queue::isCreateBlock() ) {
                      if (1/*peer_service::self_state::isLeader()*/) {
                        auto block = queue::getBlock();
                        //   ToDo send leader node
                      } else {

                        /* TODO send ping Dais Peers -> (if all timeout, this peer is altanative leader peer)
                        auto state = sendPingDaisPeers();
                        if( state == ALLTIMEOUT ) {
                          //   ToDo sumeragi.processBlock();
                          //   ToDo send block to replica();
                        }
                         */

                      }
                    }
                });
            }
        }
    }
};