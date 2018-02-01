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
#include <grpc++/grpc++.h>
#include <thread>
#include <torii/torii_service_handler.hpp>
#include <torii_utils/query_client.hpp>

namespace torii_utils {

  using iroha::protocol::Query;
  using iroha::protocol::QueryResponse;

  QuerySyncClient::QuerySyncClient(const std::string &ip, size_t port)
      : ip_(ip),
        port_(port),
        stub_(iroha::protocol::QueryService::NewStub(
            grpc::CreateChannel(ip + ":" + std::to_string(port),
                                grpc::InsecureChannelCredentials()))) {}

  QuerySyncClient::QuerySyncClient(const QuerySyncClient &rhs)
      : QuerySyncClient(rhs.ip_, rhs.port_) {}

  QuerySyncClient &QuerySyncClient::operator=(const QuerySyncClient &rhs) {
    this->ip = rhs.ip;
    this->port = rhs.port;
    this->stub_ = iroha::protocol::QueryService::NewStub(
        grpc::CreateChannel(rhs.ip + ":" + std::to_string(rhs.port),
                            grpc::InsecureChannelCredentials()));
    return *this;
  }

  /**
   * requests query to a torii server and returns response (blocking, sync)
   * @param query
   * @param response
   * @return grpc::Status
   */
  grpc::Status QuerySyncClient::Find(const iroha::protocol::Query &query,
                                     QueryResponse &response) const {
    grpc::ClientContext context;
    return stub_->Find(&context, query, &response);
  }

}  // namespace torii_utils
