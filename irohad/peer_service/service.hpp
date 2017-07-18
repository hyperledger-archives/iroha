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

#ifndef IROHA_SERVICE_HPP
#define IROHA_SERVICE_HPP

#include <grpc++/create_channel.h>
#include <peer_service.grpc.pb.h>
#include <common/byteutils.hpp>
#include <common/types.hpp>
#include <memory>
#include <random>
#include <unordered_map>
#include <uvw.hpp>
#include "connection_to.hpp"
#include "node.hpp"

namespace peerservice {

  /**
   * The task of peer service is to send heartbeat messages with given interval
   * of time to every node in the ledger.
   *
   * In future, every node may have certain topology -- "neighbours" -- a subset
   * of full network.
   */

  class PeerServiceImpl : public uvw::Emitter<PeerServiceImpl>,
                          public PeerService::Service {
   public:
    /**
     * Service constructor, which MUST be registered to grpc server builder.
     * @param cluster initial information about peers
     * @param self this node's public key
     * @param my latest known ledger state (my state)
     * @param loop uvw::Loop instance
     */
    PeerServiceImpl(const std::vector<Node>& cluster, const pubkey_t self,
                    const Heartbeat& my,
                    std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault());

    /**
     * Start heartbeating.
     */
    void start();

    /**
     * Set the information about the last block in heartbeat message.
     * @param hb
     */
    void setMyState(const Heartbeat* hb) noexcept;

    /**
     * Returns latest state among all available peers
     * @return
     */
    Heartbeat getLatestState() noexcept;
    Node getMyNode() noexcept;
    std::vector<Node> getOtherNodes() noexcept;
    std::shared_ptr<uvw::Loop> getLoop() noexcept;

    std::vector<Node> getOnlineNodes() noexcept;

    /**
     * Stop heartbeating.
     */
    void stop();

    virtual grpc::Status RequestHeartbeat(grpc::ServerContext* context,
                                          const Heartbeat* request,
                                          Heartbeat* response) override;

   private:
    void update_latest(const Heartbeat* hb) noexcept;

    std::shared_ptr<uvw::Loop> loop_;
    // latest known state. May be from any peer.
    Heartbeat latestState;
    Node self_node_;

    std::vector<Node> other_nodes_;

    std::unordered_map<pubkey_t, std::shared_ptr<ConnectionTo>> cluster_;
  };
}

#endif  // IROHA_SERVICE_HPP
