#ifndef CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
#define CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_

namespace ConsensusEventValidator {
  bool isValid(Transaction const tx);
  bool signaturesAreValid(Transaction const tx);
};

#endif  // CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
