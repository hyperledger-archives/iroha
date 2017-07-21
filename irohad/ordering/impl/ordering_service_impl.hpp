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

#ifndef IROHA_ORDERING_SERVICE_IMPL_HPP
#define IROHA_ORDERING_SERVICE_IMPL_HPP

#include "ordering/ordering_service.hpp"
#include <tbb/concurrent_queue.h>
#include "model/transaction.hpp"

namespace iroha {
  namespace ordering {

  class OrderingServiceImpl : public OrderingService{
   public:
    explicit OrderingServiceImpl(uint16_t max_size, uint16_t delay_seconds);
    void propagate_transaction(const model::Transaction &transaction) override;
    rxcpp::observable<model::Proposal> on_proposal() override;

    void generateProposal();
   private:
    tbb::concurrent_queue<model::Transaction> queue_;
    const uint16_t max_size_; // max number of txs in proposal
    const uint16_t delay_seconds_; // wait for specified time if queue is empty
    rxcpp::subjects::subject<model::Proposal> proposals_;
  };

  }
}

#endif  // IROHA_ORDERING_SERVICE_IMPL_HPP
