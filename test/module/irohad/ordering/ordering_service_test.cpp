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

#include <gtest/gtest.h>
#include "ordering/impl/ordering_service_impl.hpp"

TEST(OrderingService, OrderingServiceUseCase) {
  size_t proposal_size = 2, proposal_count = 3;
  iroha::ordering::OrderingServiceImpl ordering_service(proposal_size, 1);
  std::vector<iroha::model::Proposal> proposals;

  ordering_service.on_proposal().subscribe(
      [&proposals](auto proposal) { proposals.push_back(proposal); });

  for (size_t i = 0; i < proposal_size * proposal_count; i++) {
    iroha::model::Transaction tx;
    tx.creator_account_id = std::to_string(i);
    ordering_service.propagate_transaction(tx);
  }

  for (size_t i = 0; i < proposal_count; i++) {
    ordering_service.generateProposal();
  }

  size_t i = 0;
  ASSERT_EQ(proposals.size(), proposal_count);
  for (const auto &proposal : proposals) {
    ASSERT_EQ(proposal.transactions.size(), proposal_size);
    for (const auto &tx : proposal.transactions) {
      ASSERT_EQ(tx.creator_account_id, std::to_string(i++));
    }
  }
}

TEST(OrderingService, MultithreadTest) {
  size_t proposal_size = 2, proposal_count = 3;
  iroha::ordering::OrderingServiceImpl ordering_service(proposal_size, 1);
  std::vector<iroha::model::Proposal> proposals;

  ordering_service.on_proposal().subscribe(
      [&proposals](auto proposal) { proposals.push_back(proposal); });

  auto generate_transactions = [&ordering_service](auto from, auto to) {
    for (; from < to; ++from) {
      iroha::model::Transaction tx;
      tx.creator_account_id = std::to_string(from);
      ordering_service.propagate_transaction(tx);
    }
  };
  std::thread(generate_transactions, 0, 3).detach();
  std::thread(generate_transactions, 3, 6).detach();

  for (size_t i = 0; i < proposal_count; i++) {
    ordering_service.generateProposal();
  }

  ASSERT_EQ(proposals.size(), proposal_count);
}