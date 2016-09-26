#include "consensus_event.hpp"

#include "../model/transactions/abstract_transaction.hpp"

namespace consensus_event {
struct ConsensusEvent {

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }

    std::string getHash() {
        return self->tx::getHash();
    }
};
}  // namespace consensus_event
