/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @given transaction processor
 * @when transactions passed to processor compose proposal which is sent to peer
 * communication service
 * @then for every transaction in batches ENOUGH_SIGNATURES_COLLECTED status is
 * returned
 */
TEST_F(ConsensusStatusProcessorTest, TransactionProcessorOnProposalTest) {
  std::vector<shared_model::proto::Transaction> txs;
  for (size_t i = 0; i < proposal_size; i++) {
    auto &&tx = addSignaturesFromKeyPairs(baseTestTx(), makeKey());
    txs.push_back(tx);
  }

  EXPECT_CALL(*status_bus, publish(_))
      .Times(proposal_size)
      .WillRepeatedly(testing::Invoke([this](auto response) {
        status_map[response->transactionHash()] = response;
      }));

  // create proposal and notify about it
  auto proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder().transactions(txs).build());

  SCOPED_TRACE("Enough signatures collected status verification");
  validateStatuses<shared_model::interface::EnoughSignaturesCollectedResponse>(
      txs);
}

/**
 * @given transactions from the same batch
 * @when transactions sequence is created and propagated
 * AND all transactions were returned by pcs in proposal notifier
 * @then all transactions in batches have ENOUGH_SIGNATURES_COLLECTED status
 */
TEST_F(ConsensusStatusProcessorTest, TransactionProcessorOnProposalBatchTest) {
  using namespace shared_model::validation;
  using TxsValidator = DefaultSignedTransactionsValidator;

  auto transactions =
      framework::batch::createValidBatch(proposal_size).transactions();

  EXPECT_CALL(*status_bus, publish(_))
      .Times(proposal_size)
      .WillRepeatedly(testing::Invoke([this](auto response) {
        status_map[response->transactionHash()] = response;
      }));

  auto transaction_sequence_result =
      shared_model::interface::TransactionSequenceFactory::
          createTransactionSequence(transactions, TxsValidator());
  auto transaction_sequence =
      framework::expected::val(transaction_sequence_result).value().value;

  // create proposal from sequence transactions and notify about it
  std::vector<shared_model::proto::Transaction> proto_transactions;

  std::transform(
      transactions.begin(),
      transactions.end(),
      std::back_inserter(proto_transactions),
      [](const auto tx) {
        return *std::static_pointer_cast<shared_model::proto::Transaction>(tx);
      });

  auto proposal = std::make_shared<shared_model::proto::Proposal>(
      TestProposalBuilder().transactions(proto_transactions).build());

  SCOPED_TRACE("Enough signatures collected status verification");
  validateStatuses<shared_model::interface::EnoughSignaturesCollectedResponse>(
      proto_transactions);
}
