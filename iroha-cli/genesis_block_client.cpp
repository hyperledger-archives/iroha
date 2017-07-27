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

#include "genesis_block_client.hpp"
#include "model/converters/pb_block_factory.hpp"

#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc/grpc.h>

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>

namespace iroha_cli {

  GenesisBlockClient::GenesisBlockClient(const std::string& target_ip, const int port)
    : target_ip_(target_ip), port_(port),
      stub_(grpc::CreateChannel(target_ip_ + ":" + std::to_string(port_),
                                grpc::InsecureChannelCredentials())) {}

  grpc::Status GenesisBlockClient::SendGenesisBlock(
    const iroha::model::Block &iroha_block, iroha::protocol::ApplyGenesisBlockResponse &response) {
    grpc::ClientContext context;
    auto block_converter = iroha::model::converters::PbBlockFactory();
    auto proto_block = block_converter.serialize(iroha_block);
    stub_.SendGenesisBlock(&context, proto_block, &response);
    /*
    auto stub = iroha::protocol::GenesisBlockService::NewStub(channel);

    iroha::protocol::ToriiResponse response;
    grpc::ClientContext context;

    iroha::protocol::Block block;
    grpc::Status status = stub->SendGenesisBlock(&context, block, &response);

    if (status.ok()) {
    //  return response.message();
    }

    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    // return response.message();
    */
    return grpc::Status::OK;
  }

  void GenesisBlockClient::SendAbortGenesisBlock(const iroha::model::Block &block) {

  }
}
