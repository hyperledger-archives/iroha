
#include "event_repository.hpp"
#include "../world_state_repository.hpp"
#include <string>
#include "../../util/convert_string.hpp"
#include "../../model/transactions/abstract_transaction.hpp"
#include "../../consensus/consensus_event.hpp"

namespace repository{
namespace event {

  bool add(const std::string& hash, std::unique_ptr<consensus_event::ConsensusEvent> tx){
    if(tx->getType() == consensus_event::ConsensusEvent->tx::message){
       /*
        world_state_repository::add(
            hash,
            convert::to_string<
              consensus_event::ConsensusEvent
            >(std::move(tx))
        );
        */
    }
  }

  bool update(const std::string& hash, const consensus_event::ConsensusEvent& consensusEvent){
      
  }

  bool remove(const std::string& hash){

  }

  bool empty(){

  }

  std::vector<
    std::unique_ptr<consensus_event::ConsensusEvent>
  > findAll(){

  }

  std::unique_ptr<consensus_event::ConsensusEvent>& find(std::string hash){

  }
};
};