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

#ifndef IROHA_GENESIS_BLOCK_CLIENT_IMPL_HPP
#define IROHA_GENESIS_BLOCK_CLIENT_IMPL_HPP

#include <endpoint.grpc.pb.h>
#include <model/block.hpp>
#include "genesis_block_client.hpp"
#include "main/genesis_block_server/genesis_block_server.hpp" // GenesisBlockServicePort

namespace iroha_cli {

  class GenesisBlockClientImpl : public GenesisBlockClient {
   public:
    GenesisBlockClientImpl(){}
    void set_channel(const std::string &target_ip, const int port) override;
    grpc::Status send_genesis_block(
        const iroha::model::Block &iroha_block,
        iroha::protocol::ApplyGenesisBlockResponse &response) override;
    void send_abort_genesis_block(const iroha::model::Block &block) override;

   private:
    std::string target_ip_;
    int port_ = iroha::GenesisBlockServicePort;
  };

}  // namespace iroha_cli

#endif  // IROHA_GENESIS_BLOCK_CLIENT_IMPL_HPP
