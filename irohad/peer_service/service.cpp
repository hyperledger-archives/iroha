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
                                   const pubkey_t self, const Heartbeat &my,
                                   std::shared_ptr<uvw::Loop> loop)
      : loop_{loop} {
    update_latest(&my);

    for (auto &&node : cluster) {
      // internal cluster consists of other nodes
      if (node.pubkey != self) {
        other_nodes_.push_back(node);

        auto &&ptr = std::make_shared<ConnectionTo>(node, loop);

        // timeout handler
        ptr->timer->on<uvw::TimerEvent>(
            [ptr, this](const uvw::TimerEvent &e, auto &t) {
              ptr->ping(&this->latestState);
            });

        // heartbeat handler
        ptr->on<Heartbeat>(
            [this](const Heartbeat &hb, auto &t) { this->update_latest(&hb); });

        cluster_[node.pubkey] = std::move(ptr);

      } else {
        // this node
        self_node_ = node;
      }
    }
  }

  void PeerServiceImpl::start() {
    if (latestState.gmroot().length() != iroha::hash256_t::size()) {
      throw std::invalid_argument("add your heartbeat");
    }

    for (auto &&entry : cluster_) {
      auto &&node = entry.second;
      std::async(std::launch::async,
                 [&node]() { node->start_timer(node->next_short_timer); });
    }
  }

  grpc::Status PeerServiceImpl::RequestHeartbeat(grpc::ServerContext *context,
                                                 const Heartbeat *request,
                                                 Heartbeat *response) {
    printf("[ledger: %d] %s is requesting our heartbeat... ",
           (int)this->latestState.height(), context->peer().c_str());
    // TODO: handle
    try {
      // TODO: authenticate peer by pubkey and ip. Now we
      // authenticate by pubkey
      auto &&pub = iroha::to_blob<pubkey_t::size()>(
          request->pubkey());  // throws if size does not match

      // we have the latest known state
      if(pub == self_node_.pubkey) {
        response->CopyFrom(latestState);
        return grpc::Status::OK;
      }

      auto &&node =
          cluster_.at(pub);  // throws if no given pubkey in the map cluster_

      // TODO validate. Is this ok?  We need separate validation module
      if (request->height() < 0 ||
          request->gmroot().size() != iroha::hash256_t::size()) {
        return grpc::Status::CANCELLED;
      }

      // if we received a heartbeat with higher ledger that we have
      if (request->height() > latestState.height()) {
        update_latest(request);

        // event type: peerservice::Heartbeat
        publish(latestState);  // emit to uvw
      }

      node->make_online();

      response->CopyFrom(latestState);
      return grpc::Status::OK;

    } catch (...) {
      // TODO: handle exceptions
      printf("we received bad public key in heartbeat\n");
      return grpc::Status::CANCELLED;
    }
  }

  Heartbeat PeerServiceImpl::getLatestState() noexcept { return latestState; }

  Node PeerServiceImpl::getMyNode() noexcept { return self_node_; }

  std::shared_ptr<uvw::Loop> PeerServiceImpl::getLoop() noexcept {
    return loop_;
  }

  std::vector<Node> PeerServiceImpl::getOnlineNodes() noexcept {
    std::vector<Node> ret(other_nodes_.size());
    for (auto &&entry : cluster_) {
      auto &&node = entry.second;
      if (node != nullptr && node->online) {
        ret.push_back(node->node);
      }
    }

    return ret;
  }

  void PeerServiceImpl::setMyState(const Heartbeat *hb) noexcept {
    // if latest known state height < than new state
    update_latest(hb);
  }

  std::vector<Node> PeerServiceImpl::getOtherNodes() noexcept {
    return other_nodes_;
  }

  void PeerServiceImpl::update_latest(const Heartbeat *hb) noexcept {
    if (hb != nullptr && hb->height() > latestState.height()) {
      // TODO  change to logger
      printf("Previous ledger: %d, new ledger: %d\n", (int)latestState.height(),
             (int)hb->height());
      latestState.CopyFrom(*hb);
    }
  }
}
