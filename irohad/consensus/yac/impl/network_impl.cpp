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

#include "consensus/yac/impl/network_impl.hpp"
#include <grpc++/grpc++.h>

namespace iroha {
  namespace consensus {
    namespace yac {

      NetworkImpl::NetworkImpl(const std::string &address,
                               const std::vector<model::Peer> &peers)
          : address_(address), thread_(&NetworkImpl::asyncCompleteRpc, this) {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        server_ = builder.BuildAndStart();
        for (const auto &peer : peers) {
          peers_[peer] = proto::Yac::NewStub(grpc::CreateChannel(
              peer.address, grpc::InsecureChannelCredentials()));
        }
      }

      void NetworkImpl::subscribe(
          std::shared_ptr<YacNetworkNotifications> handler) {
        handler_ = handler;
      }

      void NetworkImpl::send_commit(model::Peer to, CommitMessage commit) {
        proto::Commit request;
        for (const auto &vote : commit.votes) {
          auto pb_vote = request.add_votes();
          auto hash = pb_vote->mutable_hash();
          hash->set_block(vote.hash.block_hash);
          hash->set_proposal(vote.hash.proposal_hash);
          auto signature = pb_vote->mutable_signature();
          signature->set_signature(vote.signature.signature.data(),
                                   vote.signature.signature.size());
          signature->set_pubkey(vote.signature.pubkey.data(),
                                vote.signature.pubkey.size());
        }

        auto call = new AsyncClientCall;

        call->response_reader =
            peers_.at(to)->AsyncSendCommit(&call->context, request, &cq_);

        call->response_reader->Finish(&call->reply, &call->status, call);
      }

      void NetworkImpl::send_reject(model::Peer to, RejectMessage reject) {
        proto::Reject request;
        for (const auto &vote : reject.votes) {
          auto pb_vote = request.add_votes();
          auto hash = pb_vote->mutable_hash();
          hash->set_block(vote.hash.block_hash);
          hash->set_proposal(vote.hash.proposal_hash);
          auto signature = pb_vote->mutable_signature();
          signature->set_signature(vote.signature.signature.data(),
                                   vote.signature.signature.size());
          signature->set_pubkey(vote.signature.pubkey.data(),
                                vote.signature.pubkey.size());
        }

        auto call = new AsyncClientCall;

        call->response_reader =
            peers_.at(to)->AsyncSendReject(&call->context, request, &cq_);

        call->response_reader->Finish(&call->reply, &call->status, call);
      }

      void NetworkImpl::send_vote(model::Peer to, VoteMessage vote) {
        proto::Vote request;
        auto hash = request.mutable_hash();
        hash->set_block(vote.hash.block_hash);
        hash->set_proposal(vote.hash.proposal_hash);
        auto signature = request.mutable_signature();
        signature->set_signature(vote.signature.signature.data(),
                                 vote.signature.signature.size());
        signature->set_pubkey(vote.signature.pubkey.data(),
                              vote.signature.pubkey.size());

        auto call = new AsyncClientCall;

        call->response_reader =
            peers_.at(to)->AsyncSendVote(&call->context, request, &cq_);

        call->response_reader->Finish(&call->reply, &call->status, call);
      }

      grpc::Status NetworkImpl::SendVote(
          ::grpc::ServerContext *context,
          const ::iroha::consensus::yac::proto::Vote *request,
          ::google::protobuf::Empty *response) {
        model::Peer peer;
        peer.address = context->peer();

        VoteMessage vote;
        vote.hash.proposal_hash = request->hash().proposal();
        vote.hash.block_hash = request->hash().block();
        std::copy(request->signature().signature().begin(),
                  request->signature().signature().end(),
                  vote.signature.signature.begin());
        std::copy(request->signature().pubkey().begin(),
                  request->signature().pubkey().end(),
                  vote.signature.pubkey.begin());

        handler_->on_vote(peer, vote);
        return grpc::Status::OK;
      }

      grpc::Status NetworkImpl::SendCommit(
          ::grpc::ServerContext *context,
          const ::iroha::consensus::yac::proto::Commit *request,
          ::google::protobuf::Empty *response) {
        model::Peer peer;
        peer.address = context->peer();

        CommitMessage commit;
        for (const auto &pb_vote : request->votes()) {
          VoteMessage vote;
          vote.hash.proposal_hash = pb_vote.hash().proposal();
          vote.hash.block_hash = pb_vote.hash().block();
          std::copy(pb_vote.signature().signature().begin(),
                    pb_vote.signature().signature().end(),
                    vote.signature.signature.begin());
          std::copy(pb_vote.signature().pubkey().begin(),
                    pb_vote.signature().pubkey().end(),
                    vote.signature.pubkey.begin());
          commit.votes.push_back(vote);
        }

        handler_->on_commit(peer, commit);
        return grpc::Status::OK;
      }

      grpc::Status NetworkImpl::SendReject(
          ::grpc::ServerContext *context,
          const ::iroha::consensus::yac::proto::Reject *request,
          ::google::protobuf::Empty *response) {
        model::Peer peer;
        peer.address = context->peer();

        RejectMessage reject;
        for (const auto &pb_vote : request->votes()) {
          VoteMessage vote;
          vote.hash.proposal_hash = pb_vote.hash().proposal();
          vote.hash.block_hash = pb_vote.hash().block();
          std::copy(pb_vote.signature().signature().begin(),
                    pb_vote.signature().signature().end(),
                    vote.signature.signature.begin());
          std::copy(pb_vote.signature().pubkey().begin(),
                    pb_vote.signature().pubkey().end(),
                    vote.signature.pubkey.begin());
          reject.votes.push_back(vote);
        }

        handler_->on_reject(peer, reject);
        return grpc::Status::OK;
      }

      NetworkImpl::~NetworkImpl() {
        cq_.Shutdown();
        if (thread_.joinable()) {
          thread_.join();
        }
      }

      void NetworkImpl::asyncCompleteRpc() {
        void *got_tag;
        auto ok = false;
        while (cq_.Next(&got_tag, &ok)) {
          auto call = static_cast<AsyncClientCall *>(got_tag);

          delete call;
        }
      }

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha