#include "ConsensusEventValidator.hpp"

#include "../infra/ConsensusEvent.hpp"
#include "../crypto/Signature.hpp"

namespace ConsensusEventValidator {

bool isValid(ConsensusEvent const event) {
    return signaturesAreValid; // TODO: add more tests
}

bool signaturesAreValid(ConsensusEvent event) {
    tx->signatures::foreach { // TODO: learn c++ syntax for this!
        if (!signature::isValid()) { // TODO: fix this syntax
            return false;
        }
    }
    return true;
}

};  // namespace ConsensusEventValidator
