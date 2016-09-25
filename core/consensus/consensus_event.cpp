#include "ConsensusEvent.hpp"

#include "../domain/transactions/abstract_transaction.hpp"

namespace ConsensusEvent {
struct ConsensusEvent {

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }

    void addSignature(std::string const signature) {
        txSignatures::push_back(signature);
    }
};
}  // namespace ConsensusEvent
