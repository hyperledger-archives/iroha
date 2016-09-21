#ifndef CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
#define CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_

namespace transaction_validator {
  bool isValid(AbstractTransaction const tx);
  bool signaturesAreValid(AbstractTransaction const tx);
  bool validForType(AbstractTransaction const tx);
};  // namespace transaction_validator

#endif  // CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
