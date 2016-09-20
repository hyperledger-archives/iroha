#ifndef CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
#define CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_

namespace transaction_validator {
  bool isValid(Transaction const tx);
  bool signaturesAreValid(Transaction const tx);
};  // namespace transaction_validator

#endif  // CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
