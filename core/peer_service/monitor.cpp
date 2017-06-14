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

#include <peer_service/monitor.hpp>
#include <peer_service/self_state.hpp>


#include <algorithm>

namespace peer_service{
  namespace monitor{

    std::shared_ptr<Node> getCurrentLeader(){
      return getActivePeerAt(0);
    }
    std::string getCurrentLeaderIp(){
      return getActivePeerAt(0)->_ip;
    }


    void initialize(){
      if( !_peer_list.empty() ) return;
      // TODO Read config.json

      // At First myself only
      _peer_list.emplace_back(
          std::make_shared<Node>(
              self_state::getIp(),
              self_state::getPublicKey(),
              self_state::getName(),
              self_state::getTrust(),
              self_state::getState()
          )
      );
    }

    size_t getMaxFaulty(){
      return std::max(0, (getActivePeerSize() - 1) / 3);
    }

    Nodes getAllPeerList(){
      return _peer_list;
    }
    std::shared_ptr<Node> getPeerAt(unsigned int index){
      try {
        return _peer_list.at(index);
      } catch( const std::out_of_range& oor ){
        // TODO Out ot Range Exception
      }
    }
    std::vector<std::string> getAllIpList(){
      std::vector<std::string> ret;
      for( auto v : _peer_list )
        ret.emplace_back( v->_ip );
      return ret;
    }

    Nodes getActivePeerList(){
      return _active_peer_list;
    }
    std::shared_ptr<Node> getActivePeerAt(unsigned int index){
      try {
        return _active_peer_list.at(index);
      } catch( const std::out_of_range& oor ){
        // TODO Out ot Range Exception
      }
    }
    std::vector<std::string> getActiveIpList(){
      std::vector<std::string> ret;
      for( auto v : _active_peer_list )
        ret.emplace_back( v->_ip );
      return ret;
    }
    int getActivePeerSize(){
      return _active_peer_list.size();
    }



    bool isExistIP(const std::string &ip){
      return findPeerIP(ip) != _peer_list.end();
    }
    bool isExistPublicKey(const std::string &publicKey){
      return findPeerPublicKey(publicKey) != _peer_list.end();
    }

    Nodes::iterator findPeerIP(const std::string &ip){
      initialize();
      return std::find_if(_peer_list.begin(), _peer_list.end(),
                          [&ip](const auto &p) { return p->_ip == ip; });
    }
    Nodes::iterator findPeerPublicKey(const std::string &publicKey){
      initialize();
      return std::find_if(
          _peer_list.begin(), _peer_list.end(),
          [&publicKey](const auto &p) { return p->_public_key == publicKey; });
    }

  };  // namespace monitor
};