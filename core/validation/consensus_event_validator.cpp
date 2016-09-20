#include "ConsensusEventValidator.hpp"

#include "../infra/ConsensusEvent.hpp"
#include "../crypto/Signature.hpp"

namespace ConsensusEventValidator {

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

};  // namespace ConsensusEventValidator
