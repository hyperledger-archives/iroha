/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <peer_service/peer_service.hpp>

namespace iroha{
  namespace peer_service {

    PeerService::PeerService() {
      self_ = SelfStatus();
    }

    /**
     * @return List of peers that therefore permutation.
     */
    std::vector<Peer> PeerService::getPermutationPeers(){
      return std::vector<Peer>();
    }

    /**
     * @param i index
     * @return A i-th peer that therefore permutation.
     */
    Peer PeerService::getPermutationAt(int i){
      return Peer();
    }

    /**
     * @return List of peers that is used by ordering service.
     */
    std::vector<Peer> PeerService::getOrderingPeers(){
      return std::vector<Peer>();
    }

    /**
     * @return List of peers that is used by ordering service and is that will be send sumeragi.
     */
    std::vector<Peer> PeerService::getActiveOrderingPeers(){
      return std::vector<Peer>();
    }


    /**
     * When on_porposal sends on_commit, it is called.
     * It check signs from Block, and identify dead peers which It throw to issue Peer::Remove transaction.
     * @param commited_block commited block with signs
     */
    void PeerService::DiesRemove(const iroha::model::Block& commited_block){
    }

    /**
     * When on_commit, it is called.
     * It change peer oreder.
     */
    void PeerService::changePermutation(){

    }


    /**
     * When commit fails, it is called.
     * It throw to issue Peer::Stop(Self) transaction.
     */
    void PeerService::selfStop(){

    }

    /**
     * When commit successes and state of self peer is UnSynced, It is called.
     * It throw to issue Peer::Activate(self) transaction.
     */
    void PeerService::selfActivate(){

    }



    void PeerService::issueStop(std::string ip,Peer &stop_peer){

    }
    void PeerService::issueActivate(std::string ip,Peer &activate_peer){

    }

  }
}