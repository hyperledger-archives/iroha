/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "network/impl/peer_communication_service_impl.hpp"

namespace iroha {
  namespace network {
    PeerCommunicationServiceImpl::PeerCommunicationServiceImpl(
        OrderingGate& orderingGate, synchronizer::Synchronizer& synchronizer)
        : orderingGate_(orderingGate), synchronizer_(synchronizer) {}

    void PeerCommunicationServiceImpl::propagate_transaction(
        const model::Transaction& transaction) {
      orderingGate_.propagate_transaction(transaction);
    }

    rxcpp::observable<model::Proposal> PeerCommunicationServiceImpl::on_proposal() {
      return orderingGate_.on_proposal();
    }


    rxcpp::observable<Commit> PeerCommunicationServiceImpl::on_commit() {
      return synchronizer_.on_commit_chain();
    }
  }
}
