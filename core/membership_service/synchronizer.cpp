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

//
// Created by Takumi Yamashita on 2017/04/28.
//

#include <membership_service/synchronizer.hpp>
#include <membership_service/peer_service.hpp>

#include <service/connection.hpp>
#include <service/flatbuffer_service.h>

#include <infra/config/iroha_config_with_json.hpp>

#include <utils/cache_map.hpp>
#include <utils/timer.hpp>
#include <ametsuchi/repository.hpp>
#include <time.h>


namespace peer{
  namespace sync {
    std::shared_ptr<::peer::Node> leader;
    void startSynchronizeLedger() {
      std::string default_leader_ip = config::IrohaConfigManager::getInstance().getConfigLeaderIp(
          ::peer::myself::getIp()
      );
      std::string message = "getPing!";
      std::string myip = ::peer::myself::getIp();
      auto vec = flatbuffer_service::endpoint::CreatePing(message,myip);
      auto &ping = *flatbuffers::GetRoot<iroha::Ping>(vec.data());
      connection::memberShipService::SyncImpl::getPeers::send(default_leader_ip,ping);

      checkRootHashStep();
    }

    void checkRootHashStep() { // step1
      if( detail::checkRootHashAll() ) peerActivateStep();
      else peerStopStep();
    }

    void peerStopStep() { // step2
      if( ::peer::myself::isActive() ) {
        ::peer::myself::stop();
        ::peer::transaction::isssue::setActive(leader->ip,::peer::myself::getIp(),false);
        detail::appending();
      }
      seekStartFetchIndex();
    }

    void seekStartFetchIndex() { // step3 ( not support )
      repository::init();
    }

    void receiveTransactions() { // step4;
      // TODO call fetchStreamTransaction()
    }

    void peerActivateStep() { // step5;
      if( ::peer::myself::isActive() ) return;
      ::peer::myself::activate();
      ::peer::transaction::isssue::setActive(leader->ip,::peer::myself::getIp(),true);
    }

    namespace detail{

      structure::CacheMap<size_t,const iroha::Transaction*> temp_tx_;
      size_t current_;
      time_t upd_time_;

      // if roothash is trust roothash, return true. othrewise return false.
      bool checkRootHashAll(){
        std::string root_hash = repository::getMerkleRoot();
        std::string myip = ::peer::myself::getPublicKey();

        auto vec = flatbuffer_service::endpoint::CreatePing(root_hash,myip);
        auto &ping = *flatbuffers::GetRoot<::iroha::Ping>(vec.data());
        if( connection::memberShipService::SyncImpl::checkHash::send(leader->ip, ping) )
          return true;
        return false;
      }
      bool append_temporary(size_t tx_id,const iroha::Transaction* tx){
        temp_tx_.set( tx_id, tx );
      }
      SYNCHRO_RESULT append(){
        size_t old_current = current_;
        while( temp_tx_.count(current_) ) {
          auto &ap_tx = *temp_tx_[current_];
          repository::append(ap_tx);
          current_++;
        }
        if( old_current != current_) {
          if( checkRootHashAll() ) return SYNCHRO_RESULT::APPEND_FINISHED;
        }
        if( !temp_tx_.empty() ){ // if started downlaoding
          // if elapsed that tiem is updated more than 2 sec and cache has more than index tx.
          if( time(NULL) - upd_time_ > 2 && temp_tx_.getMaxKey() > current_ )
            return SYNCHRO_RESULT::APPEND_ERROR;
        }
        return SYNCHRO_RESULT::APPEND_ONGOING;
      }
      void appending(){
        clearCache();

        while( !::peer::myself::isActive() ) {
          timer::waitTimer(1000);
          switch( append() ) {
            case SYNCHRO_RESULT::APPEND_ERROR:
              checkRootHashAll();
              return;
            case SYNCHRO_RESULT::APPEND_FINISHED:
              peerActivateStep();
              return;
            case SYNCHRO_RESULT::APPEND_ONGOING:
              break;
          }
        }

      }
      void clearCache(){
        current_ = 0;
        temp_tx_.clear();
      }
    } // namespace datail


  } // namespace sync
} // namespace peer