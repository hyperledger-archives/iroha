#ifndef CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
#define CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_

namespace ConsensusEventValidator {
  bool isValid(ConsensusEvent const event);
  bool signaturesAreValid(ConsensusEvent const event);
};

#endif  // CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
