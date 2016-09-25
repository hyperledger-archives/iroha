#include "signatory_delete_transaction.hpp"

namespace signatory_delete_transaction {
class SignatoryDeleteTransaction : public AbstractTransaction {
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
};  // namespace signatory_delete_transaction
