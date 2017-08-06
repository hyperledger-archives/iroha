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

#include "ordering/impl/ordering_gate_impl.hpp"

namespace iroha {
  namespace ordering {

    OrderingGateImpl::OrderingGateImpl(const std::string &server_address)
        : client_(proto::OrderingService::NewStub(grpc::CreateChannel(
              server_address, grpc::InsecureChannelCredentials()))) {
      log_ = logger::log("OrderingGate");
    }

    void OrderingGateImpl::propagate_transaction(
        std::shared_ptr<const model::Transaction> transaction) {
      log_->info("propagate tx");
      auto call = new AsyncClientCall;

      call->response_reader = client_->AsyncSendTransaction(
          &call->context, factory_.serialize(*transaction), &cq_);

      call->response_reader->Finish(&call->reply, &call->status, call);
    }

    rxcpp::observable<model::Proposal> OrderingGateImpl::on_proposal() {
      return proposals_.get_observable();
    }

    grpc::Status OrderingGateImpl::SendProposal(
        ::grpc::ServerContext *context, const proto::Proposal *request,
        ::google::protobuf::Empty *response) {
      log_->info("receive proposal");
      // auto removes const qualifier of model::Proposal.transactions
      auto transactions =
          decltype(std::declval<model::Proposal>().transactions)();
      for (const auto &tx : request->transactions()) {
        transactions.push_back(*factory_.deserialize(tx));
      }
      log_->info("transactions in proposal: {}", transactions.size());

      model::Proposal proposal(transactions);
      proposal.height = request->height();
      handleProposal(std::move(proposal));

      return grpc::Status::OK;
    }

    void OrderingGateImpl::handleProposal(model::Proposal &&proposal) {
      proposals_.get_subscriber().on_next(proposal);
    }
  }  // namespace ordering
}  // namespace iroha
