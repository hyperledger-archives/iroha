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

#include "genesis_block_server.hpp"
#include "model/converters/pb_block_factory.hpp"
#include "timer/timer.hpp"

namespace iroha {
  grpc::Status GenesisBlockService::SendGenesisBlock(
      grpc::ServerContext* context, const iroha::protocol::Block* request,
      iroha::protocol::ApplyGenesisBlockResponse* response) {
    auto converter = iroha::model::converters::PbBlockFactory();
    auto iroha_block = converter.deserialize(*request);
    auto success = processor_.genesis_block_handle(iroha_block);
    response->set_applied(success ? iroha::protocol::APPLY_SUCCESS
                                  : iroha::protocol::APPLY_FAILURE);

    // GenesisBlockServer shuts down itself after sending response to iroha_cli.
    timer::setAwkTimer(2000, [this] { server_runner_->shutdown(); });
    return grpc::Status::OK;
  }

  /**
   * runs genesis block server
   * @param ip
   * @param port
   */
  void GenesisBlockServerRunner::run(const std::string& ip, const int port) {
    std::string server_address(ip + ":" + std::to_string(port));
    iroha::GenesisBlockService service(processor_, this);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server_ = builder.BuildAndStart();
    // TODO log server start
    server_->Wait();
  }

  /**
   * shuts down server if received genesis block.
   */
  void GenesisBlockServerRunner::shutdown() { server_->Shutdown(); }

}  // namespace iroha
