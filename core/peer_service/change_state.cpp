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
#include <peer_service/change_state.hpp>
#include <peer_service/monitor.hpp>

#include <unordered_set>

namespace peer_service{
    namespace change_state{

      // This scope is issue transaction
      namespace transtion {
        // invoke to issue transaction
        void add(const std::string &ip, const Node &){

        }
        void remove(const std::string &ip, const std::string &){

        }
        void setTrust(const std::string &ip, const std::string &, const double &){

        }
        void changeTrust(const std::string &ip, const std::string &, const double &){

        }
        void setActive(const std::string &ip, const std::string &, const State state){

        }
      }

      // This scope is validation
      namespace validation {
        bool add(const Node &peer){
          if (monitor::isExistIP(peer._ip))
            return false;
          if (monitor::isExistPublicKey(peer._public_key))
            return false;
          return true;
        }
        bool remove(const std::string &publicKey){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          return true;
        }
        bool setTrust(const std::string &publicKey, const double &trust){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          return true;
        }
        bool changeTrust(const std::string &publicKey, const double &trust){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          return true;
        }
        bool setActive(const std::string &publicKey, const State state){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          return true;
        }
      }

      // This scope is runtime
      namespace runtime {
        bool add(const Node &peer){
          if (monitor::isExistIP(peer._ip))
            return false;
          if (monitor::isExistPublicKey(peer._public_key))
            return false;
          _peer_list.emplace_back( std::make_shared<Node>(peer) );
          return true;
        }
        bool remove(const std::string &publicKey){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          _peer_list.erase( monitor::findPeerPublicKey(publicKey) );
          return true;
        }
        bool setTrust(const std::string &publicKey, const double &trust){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          monitor::findPeerPublicKey(publicKey)->get()->_trust = trust;
          return true;
        }
        bool changeTrust(const std::string &publicKey, const double &trust){
          if (!monitor::isExistPublicKey(publicKey))
            return false;
          monitor::findPeerPublicKey(publicKey)->get()->_trust += trust;
          return true;
        }
        bool setActive(const std::string &publicKey, const State state){
          if (!monitor::isExistPublicKey(publicKey))
            return false;

          monitor::findPeerPublicKey(publicKey)->get()->_state = state;
          update();
        }


        void update(){
          std::unordered_set<std::string> tmp_active;

          for( auto it = _active_peer_list.begin(); it != _active_peer_list.end(); it++ ){
            if( it->get()->_state != ACTIVE )
              it = _active_peer_list.erase(it);
            else
              tmp_active.insert( it->get()->_public_key );
          }

          for( auto it = _peer_list.begin(); it != _peer_list.end(); it++ ){
            if( it->get()->_state == ACTIVE && !tmp_active.count(it->get()->_public_key) )
              _active_peer_list.emplace_back( *it );
          }
        }
      }

    };
};

