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

#include "ordering/impl/ordering_service_impl.hpp"
#include <grpc++/grpc++.h>

/**
 * Will be published when transaction is received.
 */
struct TransactionEvent {};

namespace iroha {
  namespace ordering {
    OrderingServiceImpl::OrderingServiceImpl(
        const std::vector<model::Peer> &peers, size_t max_size,
        size_t delay_milliseconds, std::shared_ptr<uvw::Loop> loop)
        : loop_(std::move(loop)),
          timer_(loop_->resource<uvw::TimerHandle>()),
          thread_(&OrderingServiceImpl::asyncCompleteRpc, this),
          max_size_(max_size),
          delay_milliseconds_(delay_milliseconds) {
      for (const auto &peer : peers) {
        peers_[peer.address] = proto::OrderingGate::NewStub(grpc::CreateChannel(
            peer.address, grpc::InsecureChannelCredentials()));
      }

      timer_->on<uvw::TimerEvent>([this](const auto &, auto &) {
        if (!queue_.empty()) {
          this->generateProposal();
        }
        timer_->start(uvw::TimerHandle::Time(delay_milliseconds_),
                      uvw::TimerHandle::Time(0));
      });

      this->on<TransactionEvent>([this](const auto &, auto &) {
        if (queue_.unsafe_size() >= max_size_) {
          timer_->stop();
          this->generateProposal();
          timer_->start(uvw::TimerHandle::Time(delay_milliseconds_),
                        uvw::TimerHandle::Time(0));
        }
      });

      timer_->start(uvw::TimerHandle::Time(delay_milliseconds_),
                    uvw::TimerHandle::Time(0));
    }

    grpc::Status OrderingServiceImpl::SendTransaction(
        ::grpc::ServerContext *context, const protocol::Transaction *request,
        ::google::protobuf::Empty *response) {
      handleTransaction(std::move(factory_.deserialize(*request)));

      return grpc::Status::OK;
    }

    void OrderingServiceImpl::handleTransaction(
        model::Transaction &&transaction) {
      queue_.push(transaction);

      publish(TransactionEvent{});
    }

    void OrderingServiceImpl::generateProposal() {
      auto txs = decltype(std::declval<model::Proposal>().transactions)();
      for (model::Transaction tx;
           txs.size() < max_size_ && queue_.try_pop(tx);) {
        txs.push_back(std::move(tx));
      }

      publishProposal(model::Proposal(txs));
    }

    void OrderingServiceImpl::publishProposal(model::Proposal &&proposal) {
      auto pb_proposal = proto::Proposal();
      for (const auto &tx : proposal.transactions) {
        auto pb_tx = pb_proposal.add_transactions();
        new (pb_tx) protocol::Transaction(factory_.serialize(tx));
      }

      for (const auto &peer : peers_) {
        auto call = new AsyncClientCall;

        call->response_reader =
            peer.second->AsyncSendProposal(&call->context, pb_proposal, &cq_);

        call->response_reader->Finish(&call->reply, &call->status, call);
      }
    }

    OrderingServiceImpl::~OrderingServiceImpl() {
      timer_->close();
      cq_.Shutdown();
      if (thread_.joinable()) {
        thread_.join();
      }
    }

    void OrderingServiceImpl::asyncCompleteRpc() {
      void *got_tag;
      auto ok = false;
      while (cq_.Next(&got_tag, &ok)) {
        auto call = static_cast<AsyncClientCall *>(got_tag);

        delete call;
      }
    }
  }
}