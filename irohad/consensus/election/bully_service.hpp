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

#ifndef IROHA_BULLY_HPP
#define IROHA_BULLY_HPP

#include <election.grpc.pb.h>
#include <messages.hpp>
#include <uvw.hpp>

namespace consensus {
  namespace election {

    using Status = grpc::Status;
    using ServerContext = grpc::ServerContext;

    class Bully final : public ElectionService::Service {
     public:
      virtual Status Victory(ServerContext* context, const Coordinator* request,
                             Void* response) override;
      virtual Status Elect(ServerContext* context, const Election* request,
                           Answer* response) override;
    };
  }
}

#endif  // IROHA_BULLY_HPP
