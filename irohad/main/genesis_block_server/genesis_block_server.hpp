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

#ifndef IROHA_GENESIS_BLOCK_SERVICE_HPP
#define IROHA_GENESIS_BLOCK_SERVICE_HPP

#include <endpoint.grpc.pb.h>
#include <grpc++/grpc++.h>
#include <model/block.hpp>
#include "genesis_block_processor.hpp"

namespace iroha {

  constexpr int GenesisBlockServicePort = 50090;

  class GenesisBlockServerRunner;

  class GenesisBlockService final
      : public iroha::protocol::GenesisBlockService::Service {
   public:
    GenesisBlockService(GenesisBlockProcessor &processor,
                        GenesisBlockServerRunner *server_runner)
        : processor_(processor), server_runner_(server_runner) {}

    grpc::Status SendGenesisBlock(
        grpc::ServerContext *context, const iroha::protocol::Block *request,
        iroha::protocol::ApplyGenesisBlockResponse *response) override;

   private:
    GenesisBlockProcessor &processor_;
    GenesisBlockServerRunner *server_runner_;
  };

  class GenesisBlockServerRunner {
   public:
    explicit GenesisBlockServerRunner(iroha::GenesisBlockProcessor &processor)
        : processor_(processor) {}
    /**
     * runs genesis block server
     * @param ip
     * @param port
     */
    void run(const std::string &ip, const int port);

    /**
     * shuts down server if received genesis block.
     */
    void shutdown();

   private:
    iroha::GenesisBlockProcessor &processor_;
    std::unique_ptr<grpc::Server> server_;
  };

}  // namespace iroha

#endif  // IROHA_GENESIS_BLOCK_SERVICE_HPP
