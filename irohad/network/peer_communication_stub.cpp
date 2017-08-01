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

#include <network/peer_communication_stub.hpp>

namespace iroha {
  namespace network {

    PeerCommunicationServiceStub::PeerCommunicationServiceStub(
        network::OrderingGate &orderer,
        synchronizer::Synchronizer &synchronizer
    ) : orderer_(orderer), synchronizer_(synchronizer) {}

    void PeerCommunicationServiceStub::propagate_transaction(
        model::Transaction transaction) {
    }

    rxcpp::observable<Commit>
    PeerCommunicationServiceStub::on_commit() {
      return synchronizer_.on_commit_chain();
    }

    rxcpp::observable<model::Proposal>
    PeerCommunicationServiceStub::on_proposal() {
      return orderer_.on_proposal();
    }

  }
}
