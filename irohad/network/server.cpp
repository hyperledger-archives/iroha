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

#include "server.hpp"
#include <grpc++/grpc++.h>

namespace network {
  Server::Server(std::shared_ptr<uvw::Loop> loop)
      : loop_{loop} {

  }

  void Server::run_grpc(uvw::Addr addr, bool blocking) {
    // bind grpc service
    auto listen_on = addr.ip + ":" + std::to_string(addr.port);
    builder.AddListeningPort(listen_on,
                             grpc::InsecureServerCredentials());
    builder.RegisterService(&this->consensus_);

    auto server = builder.BuildAndStart();

    if(blocking) {
      server->Wait();
    }
  }
}
