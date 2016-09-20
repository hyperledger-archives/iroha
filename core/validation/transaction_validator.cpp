#include "transaction_validator.hpp"

#include "../domain/signature.hpp"//TODO:!
#include "../crypto/signature.hpp"

namespace transaction_validator {

void transactionValidator(/*TODO:*/) {
    logger("initialize_transactionValidator");
    //TODO:
}

bool isValid(Transaction const tx) {
    return signaturesAreValid; // TODO: add more
}

bool signaturesAreValid(Transaction const tx) {
    for (Signature const signature in tx->signatures) { 

    }
    return true; // TODO: change this!
}

};  // namespace transaction_validator
