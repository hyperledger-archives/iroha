/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "interfaces/iroha_internal/transaction_batch.hpp"

#include <algorithm>

#include "interfaces/iroha_internal/transaction_batch_template_definitions.hpp"
#include "utils/string_builder.hpp"
#include "validators/default_validator.hpp"
#include "validators/field_validator.hpp"
#include "validators/transaction_validator.hpp"
#include "validators/transactions_collection/batch_order_validator.hpp"

namespace shared_model {
  namespace interface {

    template iroha::expected::Result<TransactionBatch, std::string>
    TransactionBatch::createTransactionBatch(
        const types::SharedTxsCollectionType &transactions,
        const validation::DefaultUnsignedTransactionsValidator &validator,
        const validation::FieldValidator &field_validator);

    template iroha::expected::Result<TransactionBatch, std::string>
    TransactionBatch::createTransactionBatch(
        const types::SharedTxsCollectionType &transactions,
        const validation::DefaultSignedTransactionsValidator &validator,
        const validation::FieldValidator &field_validator);

    template iroha::expected::Result<TransactionBatch, std::string>
    TransactionBatch::createTransactionBatch(
        std::shared_ptr<Transaction> transaction,
        const validation::DefaultUnsignedTransactionValidator
            &transaction_validator,
        const validation::FieldValidator &field_validator);

    template iroha::expected::Result<TransactionBatch, std::string>
    TransactionBatch::createTransactionBatch(
        std::shared_ptr<Transaction> transaction,
        const validation::DefaultSignedTransactionValidator
            &transaction_validator,
        const validation::FieldValidator &field_validator);

    const types::SharedTxsCollectionType &TransactionBatch::transactions()
        const {
      return transactions_;
    }

    const types::HashType &TransactionBatch::reducedHash() const {
      if (not reduced_hash_) {
        reduced_hash_ = TransactionBatch::calculateReducedBatchHash(
            transactions_ | boost::adaptors::transformed([](const auto &tx) {
              return tx->reducedHash();
            }));
      }
      return reduced_hash_.value();
    }

    bool TransactionBatch::hasAllSignatures() const {
      return std::all_of(
          transactions_.begin(), transactions_.end(), [](const auto tx) {
            return boost::size(tx->signatures()) >= tx->quorum();
          });
    }

    std::string TransactionBatch::toString() const {
      return detail::PrettyStringBuilder()
          .init("Batch")
          .append("reducedHash", reducedHash().toString())
          .append("hasAllSignatures", hasAllSignatures() ? "true" : "false")
          .append("transactions")
          .appendAll(transactions(), [](auto &tx) { return tx->toString(); })
          .finalize();
    }

    bool TransactionBatch::addSignature(
        size_t number_of_tx,
        const shared_model::crypto::Signed &signed_blob,
        const shared_model::crypto::PublicKey &public_key) {
      if (number_of_tx >= transactions_.size()) {
        return false;
      } else {
        return transactions_.at(number_of_tx)
            ->addSignature(signed_blob, public_key);
      }
    }

    bool TransactionBatch::operator==(const TransactionBatch &rhs) const {
      return reducedHash() == rhs.reducedHash()
          and std::equal(transactions().begin(),
                         transactions().end(),
                         rhs.transactions().begin(),
                         rhs.transactions().end(),
                         [](auto const &left, auto const &right) {
                           return left->equalsByValue(*right);
                         });
    }

  }  // namespace interface
}  // namespace shared_model
