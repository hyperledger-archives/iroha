#include "domain_definition_transaction.hpp"

namespace domain_definition_transaction {

class DomainDefinitionTransaction : public AbstractTransaction {
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
};  // namespace domain_definition_transaction
