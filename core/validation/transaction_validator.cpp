#include "transaction_validator.hpp"

#include "../model/abstract_transaction.hpp"
#include "../crypto/signature.hpp"

namespace transaction_validator {
bool isValid(abstract_transaction::AbstractTransaction& tx) {
    return signaturesAreValid(tx) && validForType(tx);
}

bool signaturesAreValid(abstract_transaction::AbstractTransaction& tx) {
    bool areAllValid = true;
    for (Signature const sig in tx->signatures) { 
        if (!signature::verify(sig, tx::getRawData(), tx::getPublicKey) {
            areAllValid = false;
        }
    }
    return areAllValid; // TODO: change this!
}

bool validForType(abstract_transaction::AbstractTransaction& tx) {
    if (abstract_transaction::TransactionType::transfer == tx::getType) {
        TransferTransaction transferTx = static_cast<TransferTransaction>(tx);
        getBalance();
    } else if (abstract_transaction::TransactionType::addPeer == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::modifyPeer == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::removePeer == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::signatory == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::signatoryAdd == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::signatoryDelete == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::domainDefinition == tx::getType) {

    } else if (abstract_transaction::TransactionType::domainRenewal == tx::getType) {

    } else if (abstract_transaction::TransactionType::aliasDefinition == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::aliasRenewal == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::assetDefinition == tx::getType) {

    } else if (abstract_transaction::TransactionType::message == tx::getType) {

    } else if (abstract_transaction::TransactionType::chaincodeInit == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::chaincodeInvoke == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::chaincodeUpdate == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::chaincodeDestory == tx::getType) {
        return true;  //TODO-future-work
    } else if (abstract_transaction::TransactionType::interchain == tx::getType) {
        return true;  //TODO-future-work
    }
}

};  // namespace transaction_validator
