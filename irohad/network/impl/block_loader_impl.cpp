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

#include "network/impl/block_loader_impl.hpp"
#include <grpc++/create_channel.h>

using namespace iroha::network;
using namespace iroha::model;

rxcpp::observable<Block> BlockLoaderImpl::requestBlocks(Peer target_peer,
                                                        Block topBlock) {
  return rxcpp::observable<>::create<Block>(
      [this, target_peer, topBlock](auto subscriber) {
        proto::BlocksRequest request;
        grpc::ClientContext context;
        protocol::Block block;

        // request next block to our top
        request.set_height(topBlock.height + 1);

        auto reader =
            this->getPeerStub(target_peer).retrieveBlocks(&context, request);
        while (reader->Read(&block)) {
          subscriber.on_next(factory_.deserialize(block));
        }
        reader->Finish();
        subscriber.on_completed();
      });
}

proto::Loader::Stub &BlockLoaderImpl::getPeerStub(const Peer &peer) {
  auto it = peer_connections_.find(peer);
  if (it == peer_connections_.end()) {
    it = peer_connections_.insert(std::make_pair(peer, proto::Loader::NewStub(
            grpc::CreateChannel(peer.address,
                                grpc::InsecureChannelCredentials())))).first;
  }
  return *it->second;
}
