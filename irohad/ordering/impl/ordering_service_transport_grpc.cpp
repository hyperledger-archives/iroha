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
#include "ordering_service_transport_grpc.hpp"
#include <block.pb.h>
#include <grpc++/create_channel.h>
#include <ordering.grpc.pb.h>
#include <ordering.pb.h>
#include <unordered_map>
#include "ordering.pb.h"

using namespace iroha::ordering;
void iroha::ordering::OrderingServiceTransportGrpc::subscribe(
    std::shared_ptr<iroha::network::OrderingServiceNotification> subscriber) {
  subscriber_ = subscriber;
}

grpc::Status OrderingServiceTransportGrpc::onTransaction(::grpc::ServerContext *context,
                           const protocol::Transaction *request,
                           ::google::protobuf::Empty *response) {
  subscriber_->onTransaction(*factory_.deserialize(*request));
  return ::grpc::Status::OK;
}

void iroha::ordering::OrderingServiceTransportGrpc::publishProposal(
    iroha::model::Proposal &&proposal, const std::vector<std::string> &peers) {
  std::unordered_map<std::string,
                     std::unique_ptr<proto::OrderingGateTransportGrpc::Stub>>
      peers_map;

  for (const auto &peer : peers) {
    peers_map[peer] = proto::OrderingGateTransportGrpc::NewStub(
        grpc::CreateChannel(peer, grpc::InsecureChannelCredentials()));
  }

  proto::Proposal pb_proposal;
  pb_proposal.set_height(proposal.height);
  for (const auto &tx : proposal.transactions) {
    auto pb_tx = pb_proposal.add_transactions();
    new (pb_tx) protocol::Transaction(factory_.serialize(tx));
  }

  for (const auto &peer : peers_map) {
    auto call = new AsyncClientCall;

    call->response_reader =
        peer.second->AsynconProposal(&call->context, pb_proposal, &cq_);

    call->response_reader->Finish(&call->reply, &call->status, call);
  }
}
