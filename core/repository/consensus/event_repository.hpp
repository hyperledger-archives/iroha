#ifndef CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_

#include <string>
#include <memory>
#include "../../model/transactions/abstract_transaction.hpp"
#include "../../consensus/consensus_event.hpp"


namespace repository{
    namespace event {

        std::vector<
                std::unique_ptr<consensus_event::ConsensusEvent>
        > consensusEvents;


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

        std::vector<std::unique_ptr<consensus_event::ConsensusEvent>>&& findAll(){
            return std::move(consensusEvents);
        }

        std::unique_ptr<
            consensus_event::ConsensusEvent
        >&& findNext() {
            auto res = std::move(consensusEvents.front());
            consensusEvents.pop_back();
            return std::move(res);
        }

        std::unique_ptr<consensus_event::ConsensusEvent> find(std::string hash) {
            for(auto&& event : consensusEvents){
                if( event->merkleRoot == hash) return std::move(event);
            }
            return std::unique_ptr<consensus_event::ConsensusEvent>(nullptr);
        }
    };
};

#endif  // CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
