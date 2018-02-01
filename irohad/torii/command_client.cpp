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

#include "block.pb.h"
#include "torii/command_client.hpp"
#include "torii/torii_service_handler.hpp"

namespace torii {

  using iroha::protocol::ToriiResponse;
  using iroha::protocol::Transaction;

  CommandSyncClient::CommandSyncClient(const std::string &ip, size_t port)
      : ip_(ip),
        port_(port),
        stub_(iroha::protocol::CommandService::NewStub(
            grpc::CreateChannel(ip + ":" + std::to_string(port),
                                grpc::InsecureChannelCredentials()))) {}

  CommandSyncClient::CommandSyncClient(const CommandSyncClient &rhs)
      : CommandSyncClient(rhs.ip_, rhs.port_) {}

  CommandSyncClient &CommandSyncClient::operator=(
      const CommandSyncClient &rhs) {
    this->ip_ = rhs.ip_;
    this->port_ = rhs.port_;
    this->stub_ = iroha::protocol::CommandService::NewStub(
        grpc::CreateChannel(rhs.ip_ + ":" + std::to_string(rhs.port_),
                            grpc::InsecureChannelCredentials()));
    return *this;
  }

  grpc::Status CommandSyncClient::Torii(const Transaction &tx) const {
    google::protobuf::Empty a;
    grpc::ClientContext context;
    return stub_->Torii(&context, tx, &a);
  }

  grpc::Status CommandSyncClient::Status(
      const iroha::protocol::TxStatusRequest &request,
      iroha::protocol::ToriiResponse &response) const {
    grpc::ClientContext context;
    return stub_->Status(&context, request, &response);
  }

}  // namespace torii
