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

#include "service.hpp"
#include <future>

namespace peerservice {

  PeerServiceImpl::PeerServiceImpl(const std::vector<Node> &cluster,
                                   const pubkey_t self,
                                   std::shared_ptr<uvw::Loop> loop)
      : loop_{loop} {
    for (auto &&node : cluster) {
      // internal cluster consists of other nodes
      if (node.pubkey != self) {
        cluster_[node.pubkey] = std::make_shared<ConnectionTo>(node, loop);
      } else {
        self_node_ = node;
      }
    }
  }

  void PeerServiceImpl::ping() {
    for (auto &&entry : cluster_) {
      auto &&node = entry.second;
      Heartbeat hb = node->ping(&this->myHeartbeat);
      if (hb.height() > latestState.height()) {
        latestState.CopyFrom(hb);
      }
    }
  }

  grpc::Status PeerServiceImpl::RequestHeartbeat(grpc::ServerContext *context,
                                                 const Heartbeat *request,
                                                 Heartbeat *response) {
    // TODO validate. Is this ok?
    //    if(request->height() < 0 ||  request->gmroot().size() !=
    //    iroha::hash256_t::size()){
    //      return grpc::Status::CANCELLED;
    //    }

    // if we received a heartbeat with higher ledger that we have
    if (request->height() > myHeartbeat.height()) {
      latestState.CopyFrom(*request);
      ENewLedger event;
      event.data.CopyFrom(latestState);
      publish(event);  // emit to uvw
    }

    response->CopyFrom(myHeartbeat);
    return grpc::Status::OK;
  }

  Heartbeat PeerServiceImpl::getLatestState() noexcept { return latestState; }
  Heartbeat PeerServiceImpl::getMyHeartbeat() noexcept { return myHeartbeat; }

  Node PeerServiceImpl::getMyNode() noexcept { return self_node_; }

  std::shared_ptr<uvw::Loop> PeerServiceImpl::getLoop() noexcept {
    return loop_;
  }

  std::vector<Node> PeerServiceImpl::getOnlineNodes() noexcept {
    std::vector<Node> ret;
    for (auto &&entry : cluster_) {
      auto &&node = entry.second;
      if (node != nullptr && node->online) {
        ret.push_back(node->node);
      }
    }

    return ret;
  }

  void PeerServiceImpl::setMyState(Heartbeat hb) {
    // if latest known state height < than new state
    if (hb.height() > latestState.height()) {
      latestState.CopyFrom(hb);
    }

    myHeartbeat = std::move(hb);
  }
}
