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

#ifndef TORII_COMMAND_SERVICE_HPP
#define TORII_COMMAND_SERVICE_HPP

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>
#include <torii/command_service_handler.hpp>

namespace torii {

  /**
   * Actual implementation of async CommandService.
   * CommandServiceHandler::(SomeMethod)Handler calls a corresponding method in this class.
   */
  class CommandService {
  public:
    /**
     * actual implementation of async Torii in CommandService
     * @param request - Transaction
     * @param response - ToriiResponse
     * @return grpc::Status - Status::OK if succeeded. TODO(motxx): grpc::CANCELLED is not supported.
     */
    static grpc::Status ToriiAsync(
      iroha::protocol::Transaction const& request, iroha::protocol::ToriiResponse& response) {
      response.set_code(iroha::protocol::ResponseCode::OK);
      response.set_message("Torii async response");
      return grpc::Status::OK;
    }
  };

}  // namespace torii

#endif  // TORII_COMMAND_SERVICE_HPP
