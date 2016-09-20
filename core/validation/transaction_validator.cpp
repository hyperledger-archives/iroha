#include "transaction_validator.hpp"

#include "../domain/abstract_transaction.hpp"
#include "../crypto/signature.hpp"

namespace transaction_validator {
bool isValid(Transaction const tx) {
    return signaturesAreValid(tx) && validForType; // TODO: add more tests
}

bool signaturesAreValid(AbstractTransaction const tx) {
    bool areAllValid = true;
    for (Signature const sig in tx->signatures) { 
        if (!signature::verify(sig, tx::getRawData(), tx::getPublicKey) {
            areAllValid = false;
        }
    }
    return areAllValid; // TODO: change this!
}

bool validForType(AbstractTransaction const tx) {
    if (abstract_transaction::TransactionType::transfer == tx::getType) {

    } else if (abstract_transaction::TransactionType::addPeer == tx::getType) {

    } else if (abstract_transaction::TransactionType::modifyPeer == tx::getType) {

    } else if (abstract_transaction::TransactionType::removePeer == tx::getType) {

    } else if (abstract_transaction::TransactionType::signatory == tx::getType) {

    } else if (abstract_transaction::TransactionType::signatoryAdd == tx::getType) {

    } else if (abstract_transaction::TransactionType::signatoryDelete == tx::getType) {

    } else if (abstract_transaction::TransactionType::domainDefinition == tx::getType) {

    } else if (abstract_transaction::TransactionType::domainRenewal == tx::getType) {

    } else if (abstract_transaction::TransactionType::aliasDefinition == tx::getType) {

    } else if (abstract_transaction::TransactionType::aliasRenewal == tx::getType) {

    } else if (abstract_transaction::TransactionType::assetDefinition == tx::getType) {

    } else if (abstract_transaction::TransactionType::message == tx::getType) {

    } else if (abstract_transaction::TransactionType::chaincodeInit == tx::getType) {

    } else if (abstract_transaction::TransactionType::chaincodeInvoke == tx::getType) {

    } else if (abstract_transaction::TransactionType::chaincodeUpdate == tx::getType) {

    } else if (abstract_transaction::TransactionType::chaincodeDestory == tx::getType) {

    } else if (abstract_transaction::TransactionType::interchain == tx::getType) {

    }
}

};  // namespace transaction_validator
