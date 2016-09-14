#ifndef CORE_INFRA_CONSENSUSEVENT_HPP_
#define CORE_INFRA_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>

namespace ConsensusEvent {
enum class event { 
    transaction, panic
};

};  // namespace ConsensusEvent

#endif  // CORE_INFRA_CONSENSUSEVENT_HPP_
