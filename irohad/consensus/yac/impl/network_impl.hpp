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

#ifndef IROHA_NETWORK_IMPL_HPP
#define IROHA_NETWORK_IMPL_HPP

#include <atomic>
#include <thread>
#include <unordered_map>
#include "network/impl/async_grpc_client.hpp"
#include "consensus/yac/yac_network_interface.hpp"
#include "yac.grpc.pb.h"

namespace iroha {
  namespace consensus {
    namespace yac {

      class NetworkImpl : public YacNetwork,
                          public proto::Yac::Service,
                          network::AsyncGrpcClient<google::protobuf::Empty> {
       public:
        explicit NetworkImpl(const std::string &address,
                             const std::vector<model::Peer> &peers);
        void subscribe(
            std::shared_ptr<YacNetworkNotifications> handler) override;
        void send_commit(model::Peer to, CommitMessage commit) override;
        void send_reject(model::Peer to, RejectMessage reject) override;
        void send_vote(model::Peer to, VoteMessage vote) override;

        /*
         * gRPC server methods
         */
        grpc::Status SendVote(
            ::grpc::ServerContext *context,
            const ::iroha::consensus::yac::proto::Vote *request,
            ::google::protobuf::Empty *response) override;
        grpc::Status SendCommit(
            ::grpc::ServerContext *context,
            const ::iroha::consensus::yac::proto::Commit *request,
            ::google::protobuf::Empty *response) override;
        grpc::Status SendReject(
            ::grpc::ServerContext *context,
            const ::iroha::consensus::yac::proto::Reject *request,
            ::google::protobuf::Empty *response) override;

       private:
        std::string address_;
        std::unordered_map<model::Peer, std::unique_ptr<proto::Yac::Stub>>
            peers_;
        std::weak_ptr<YacNetworkNotifications> handler_;

        std::unordered_map<std::string, model::Peer> peers_addresses_;
      };

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha

#endif  // IROHA_NETWORK_IMPL_HPP
