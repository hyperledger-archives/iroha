#ifndef CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
#define CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_

#include "../model/transactions/abstract_transaction.hpp"

namespace transaction_validator {
  bool isValid(abstract_transaction::AbstractTransaction& tx);
  bool signaturesAreValid(abstract_transaction::AbstractTransaction& tx);
  bool validForType(abstract_transaction::AbstractTransaction& tx);
};  // namespace transaction_validator

#endif  // CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
