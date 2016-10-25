#include "transfer_transaction.hpp"
#include <string>

namespace transaction {

    std::string TransferTransaction::getHash() {
        return hash;
    }

    std::string TransferTransaction::getRawData() {
        //TODO
    }

    std::string TransferTransaction::getAsText() {
        //TODO
    }

    unsigned long long  TransferTransaction::getTimestamp() {
        return timestamp;
    }

    abstract_transaction::TransactionType TransferTransaction::getType() {
        return type;
    }

    TransferTransaction::TransferTransaction(
         const std::string &senderPublicKey, const std::string &receiverPublicKey,
         const std::string &domain, const std::string &asset
    ):
        AbstractTransaction(),
        senderPublicKey(senderPublicKey),
        receiverPublicKey(receiverPublicKey),
        domain(domain),
        asset(asset)
    {}


};  // namespace transaction
