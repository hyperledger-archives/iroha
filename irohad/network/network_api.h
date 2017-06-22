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
#ifndef IROHA_NETWORK_H
#define IROHA_NETWORK_H

#include "rxcpp/rx-observable.hpp"
#include "dao/dao.hpp"

namespace iroha {
  namespace network {

    /**
     * Interface provide methods for fetching useful consensus-related data.
     */
    class ConsensusPublicApi {
     public:

      /**
       * Provide proposals that shared in network for validation
       * @return observable with new proposals shared in network
       */
      virtual rxcpp::observable<iroha::dao::Proposal> on_proposal() = 0;

      /**
       * Provide blocks that passed network consensus
       * @return observable with sequence of committed blocks.
       * In common case observable<Block> will contain one element.
       * But exists scenarios when consensus provide many blocks.
       * Ex: On peer startup - wait for committed block -
       * network will provide all blocks committed after last block of current ledger.
       */
      virtual rxcpp::observable<rxcpp::observable<iroha::dao::Block>> on_commit() = 0;
    };

    /**
     * Interface for downloading blocks from network
     */
    class BlockLoaderApi {
     public:

      /**
       * Method request missed blocks from external peer retatively to top block.
       * Note, that blocks will be respond in reversed order - from the newest to top block.
       * This order required for verify blocks before storing in ledger.
       * @param from - peer for requesting blocks
       * @param topBlock - started block for downloading
       * @return observable with respond blocks
       */
      virtual rxcpp::observable<iroha::dao::Block> requestBlocks(iroha::dao::Peer &from,
                                                                 iroha::dao::Block &topBlock) = 0;
    };

    /**
     * Interface for propagation transaction in network
     */
    class TransactionPropagator {
     public:

      /**
       * Method spread transaction for other members of network
       * @param tx - transaction for propagation
       */
      virtual void propagate_transaction(iroha::dao::Transaction &tx) = 0;
    };

    /**
     * Public API interface for communication between current peer and other peers in network
     */
    class PeerToPeerNetworking
        : public TransactionPropagator, public ConsensusPublicApi {

    };

  }
}
#endif //IROHA_NETWORK_H
