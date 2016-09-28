#ifndef CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
#define CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_

#include "../consensus/consensus_event.hpp"

namespace consensus_event_validator {
  bool isValid(const consensus_event::ConsensusEvent event);
  bool signaturesAreValid(const consensus_event::ConsensusEvent event);
};  // namespace consensus_event_validator

#endif  // CORE_VALIDATION_CONSENSUSEVENTVALIDATOR_HPP_
