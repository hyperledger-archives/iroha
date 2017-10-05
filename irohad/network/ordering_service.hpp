//
// Created by Dumitru Savva on 02/10/2017.
//

#ifndef IROHA_ORDERINGSERVICE_H
#define IROHA_ORDERINGSERVICE_H

#include "network/ordering_service_transport.hpp"

namespace iroha {
  namespace network {
    class OrderingService : public network::OrderingServiceNotification {
     public:
      /**
       * Collect transactions from queue
       * Passes the generated proposal to publishProposal
       */
      virtual void generateProposal() = 0;

      /**
       * Transform model proposal to transport object and send to peers
       * @param proposal - object for propagation
       */
      virtual void publishProposal(model::Proposal &&proposal) = 0;
    };
  }
}
#endif //IROHA_ORDERINGSERVICE_H
