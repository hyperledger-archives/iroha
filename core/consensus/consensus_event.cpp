#include "consensus_event.hpp"

#include "../domain/transactions/abstract_transaction.hpp"

namespace consensus_event {
struct ConsensusEvent {

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }
};
}  // namespace consensus_event
