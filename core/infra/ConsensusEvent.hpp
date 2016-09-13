#ifndef CORE_INFRA_CONSENSUSEVENT_HPP_
#define CORE_INFRA_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>

namespace ConsensusEvent {
enum types { 
    awk, suspicion, transaction, viewChange
};//TODO: should this be here?


};

#endif  // CORE_INFRA_CONSENSUSEVENT_HPP_
