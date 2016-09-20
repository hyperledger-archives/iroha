#include "transaction_validator.hpp"

#include "../crypto/signature.hpp"

namespace transaction_validator {

void transactionValidator(AbstractTransaction const tx) {
    logger("initialize_transactionValidator");
    //TODO:
}

bool isValid(Transaction const tx) {
    return signaturesAreValid(tx); // TODO: add more
}

bool signaturesAreValid(Transaction const tx) {
    bool areAllValid = true;
    for (Signature const sig in tx->signatures) { 
        if (!signature::verify(sig, tx::getRawData(), tx::getPublicKey) {
            areAllValid = false;
        }
    }
    return areAllValid; // TODO: change this!
}

};  // namespace transaction_validator
