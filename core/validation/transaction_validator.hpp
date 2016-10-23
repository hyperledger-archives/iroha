#ifndef CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
#define CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_

#include "../model/transactions/abstract_transaction.hpp"

namespace transaction_validator {
  bool isValid(const std::unique_ptr<abstract_transaction::AbstractTransaction> tx);
  bool signaturesAreValid(const abstract_transaction::AbstractTransaction& tx);
  bool validForType(const abstract_transaction::AbstractTransaction& tx);
};  // namespace transaction_validator

#endif  // CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
