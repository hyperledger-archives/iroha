
#include "event_repository.hpp"
#include "../world_state_repository.hpp"
#include <string>

#include "../../util/convert_string.hpp"
#include "../../model/transactions/abstract_transaction.hpp"
#include "../../consensus/consensus_event.hpp"

namespace repository{
namespace event {

    bool add(const std::string &hash, std::unique_ptr<consensus_event::ConsensusEvent> tx) {
        return false;
    }

    bool update(const std::string &hash, const consensus_event::ConsensusEvent &consensusEvent) {
        return false;
    }

    bool remove(const std::string &hash) {
        return false;
    }

    bool empty() {
        return false;
    }

    std::vector<
            std::unique_ptr<consensus_event::ConsensusEvent>
    > findAll() {

    }

    std::vector<
            std::unique_ptr<consensus_event::ConsensusEvent>
    > findNext() {

    }

    std::unique_ptr<consensus_event::ConsensusEvent> &find(std::string hash) {

    }
};
};
