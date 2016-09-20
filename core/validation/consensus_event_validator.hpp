#ifndef CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
#define CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_

namespace consensus_event_validator {
  bool isValid(ConsensusEvent const event);
  bool signaturesAreValid(ConsensusEvent const event);
};  // namespace consensus_event_validator

#endif  // CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
