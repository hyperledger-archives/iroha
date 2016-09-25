#include "transfer_transaction.hpp"
#include "abstract_transfer_transaction.hpp"
#include <string>

namespace transfer_transaction {
class TransferTransaction : public AbstractTransaction {
    std::string hash;
    AbstractTransaction::TransactionType type;

    std::string getHash() {
        return hash;
    }

    std::string getRawData() {
        //TODO
    }
    
    std::string getAsText() {
        //TODO
    }
    
    unsigned long long  getTimestamp() {
        return timestamp;
    }
    
    TransactionType getType() {
        return type;
    }
};

};  // namespace transfer_transaction
