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
#include <utils/cache_map.hpp>
#include <utils/timer.hpp>


namespace peer{
  namespace sync {
    void startSynchronizeLedger() {
      checkRootHashStep();
    }

    void checkRootHashStep() { // step1
      if( detail::checkRootHashAll() ) peerActivateStep();
      else peerStopStep();
    }

    void peerStopStep() { // step2
      if( ::peer::myself::isActive() ) {
        ::peer::myself::stop();
        // TODO send PeerSetActive(false) Transaction
        // TOOD setup appending() another thread.
      }
      seekStartFetchIndex();
    }

    void seekStartFetchIndex() { // step3 ( not support )
      // TODO after integration ametsuchi
    }

    void receiveTransactions() { // step4;
      // TODO call fetchStreamTransaction()
    }

    void peerActivateStep() { // step5;
      if( ::peer::myself::isActive() ) return;
      ::peer::myself::activate();
      // TODO send PeerSetActive(true) Transaction
    }

    namespace detail{

      structure::CacheMap<size_t,iroha::Transaction*> temp_tx_;
      size_t current_;

      // if roothash is trust roothash, return true. othrewise return false.
      bool checkRootHashAll(/*merkle::hash_t*/){
        // TODO after integration ametsuchi and implement rpc_service
      }
      bool append_temporary(size_t tx_id,iroha::Transaction* tx){
        temp_tx_.set( tx_id, tx );
      }
      SYNCHRO_RESULT append(){
        // TODO after integreation ametsuchi
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