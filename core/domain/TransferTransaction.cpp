#include "TransferTransaction.hpp"
#include "AbstractTransferTransaction.hpp"
#include <string>

namespace TransferTransaction {

class TransferTransaction : public AbstractTransaction {
    std::string hash;
    AbstractTransaction::TransactionType type;

    MSGPACK_DEFINE(hash, type, senderPublicKey, receiverPublicKey, domain, asset, amount);
 };
    
};  // namespace TransferTransaction
