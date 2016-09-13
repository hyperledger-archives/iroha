#include "TransactionValidator.hpp"

#include "../domain/Signature.hpp"//TODO:!
#include "../crypto/Signature.hpp"

namespace TransactionValidator {

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

};  // namespace TransactionValidator
