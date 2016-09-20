#include "consensus_event_validator.hpp"

#include "../infra/consensus_event.hpp"
#include "../crypto/signature.hpp"

namespace consensus_event_validator {

bool isValid(ConsensusEvent const event) {
    return signaturesAreValid; // TODO: add more tests
}

bool signaturesAreValid(ConsensusEvent const event) {
    for (std::<shared_ptr>Signature const signature : event->signatures) {
        if (!signature::isValid()) {
            return false;
        }
    }
    return true;
}

};  // namespace consensus_event_validator
