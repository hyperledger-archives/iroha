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

#include "genesis_block_service.hpp"
#include "model/converters/pb_block_factory.hpp"

namespace iroha {
  grpc::Status GenesisBlockService::SendGenesisBlock(
      grpc::ServerContext* context, const iroha::protocol::Block* request,
      iroha::protocol::ApplyGenesisBlockResponse* response) {
    auto converter = iroha::model::converters::PbBlockFactory();
    auto iroha_block = converter.deserialize(*request);
    auto success = processor_.genesis_block_handle(iroha_block);
    response->set_applied(success ? iroha::protocol::APPLY_SUCCESS
                                  : iroha::protocol::APPLY_FAILURE);
    return grpc::Status::OK;
  }
}  // namespace iroha
