#include "ConsensusEventValidator.hpp"

#include "../infra/ConsensusEvent.hpp"
#include "../crypto/Signature.hpp"

namespace ConsensusEventValidator {

struct Context {
    //TODO:
};

void transactionValidator(/*TODO:*/) {
    logger("initialize_transactionValidator");
    //TODO:
}

bool isValid(ConsensusEvent const tx) {
    return signaturesAreValid; // TODO: add more
}

bool signaturesAreValid(Transaction const tx) {
    tx->signatures::foreach { // TODO: learn c++ syntax for this!

    }
    return true; // TODO: change this!
}

};  // namespace ConsensusEventValidator
