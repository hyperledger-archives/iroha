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

#include <block.pb.h>
#include "service.hpp"

namespace connection {
  namespace consensus {

    using iroha::protocol::Block;
    using iroha::protocol::VerifyResponse;

    std::function<void(const Block&)> dispatchToSumeragi;

    void receive(const std::function<void(const Block&)>& func) {
      dispatchToSumeragi = func;
    }

    grpc::Status SumeragiService::Verify(grpc::ClientContext* context,
                                         const Block& request,
                                         VerifyResponse* response) {
      return grpc::Status::OK;
    }
  }  // namespace consensus
}  // namespace connection
