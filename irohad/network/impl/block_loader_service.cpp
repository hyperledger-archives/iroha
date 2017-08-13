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

#include "network/impl/block_loader_service.hpp"

using namespace iroha::network;
using namespace iroha::ametsuchi;

BlockLoaderService::BlockLoaderService(std::shared_ptr<BlockQuery> storage)
    : storage_(std::move(storage)) {}

grpc::Status BlockLoaderService::retrieveBlocks(
    ::grpc::ServerContext *context, const proto::BlocksRequest *request,
    ::grpc::ServerWriter<::iroha::protocol::Block> *writer) {
  storage_->getBlocksFrom(request->height())
      .map([this](auto block) { return factory_.serialize(block); })
      .as_blocking()
      .subscribe([writer](auto block) { writer->Write(block); });
  return grpc::Status::OK;
}
