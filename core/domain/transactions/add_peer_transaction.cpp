#include "add_peer_transaction.hpp"

namespace add_peer_transaction {
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
};  // namespace add_peer_transaction