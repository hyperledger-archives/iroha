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

#include <thread>

#include <grpc++/grpc++.h>
#include <network/grpc_call.hpp>

#include "block.pb.h"
#include "torii/command_client.hpp"
#include "torii/torii_service_handler.hpp"

namespace torii {

  using iroha::protocol::ToriiResponse;
  using iroha::protocol::Transaction;

  CommandSyncClient::CommandSyncClient(std::string ip, int port)
      : stub_(iroha::protocol::CommandService::NewStub(
            grpc::CreateChannel(ip + ":" + std::to_string(port),
                                grpc::InsecureChannelCredentials()))) {}

  CommandSyncClient::~CommandSyncClient() {
    completionQueue_.Shutdown();
  }

  const char *kQueueNextError = "CompletionQueue::Next() returns error";

  /**
   * requests tx to a torii server and returns response (blocking, sync)
   * @param tx
   * @param response - returns ToriiResponse if succeeded
   * @return grpc::Status - returns connection is success or not.
   */
  grpc::Status CommandSyncClient::Torii(const Transaction &tx) {
    google::protobuf::Empty a;
    return stub_->Torii(&context_, tx, &a);
  }

  grpc::Status CommandSyncClient::Status(
      const iroha::protocol::TxStatusRequest &request,
      iroha::protocol::ToriiResponse &response) {
    return stub_->Status(&context_, request, &response);
  }

}  // namespace torii
