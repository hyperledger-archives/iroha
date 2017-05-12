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

#ifndef IROHA_SYNCHRONIZER_H
#define IROHA_SYNCHRONIZER_H

#include <transaction_generated.h>
#include <utils/cache_map.hpp>
#include <string>

namespace peer{
  namespace sync{


    void startSynchronizeLedger();
    void checkRootHashStep(); // step1
    void peerStopStep(); // step2
    void seekStartFetchIndex(); // step3 ( not support )
    void receiveTransactions(); // step4;
    void peerActivateStep(); // step5;

    enum SYNCHRO_RESULT {
      APPEND_ONGOING,
      APPEND_ERROR,
      APPEND_FINISHED,
    };

    namespace detail{
      // if roothash is trust roothash, return true. othrewise return false.
      bool checkRootHashAll();

      bool append_temporary(size_t,const iroha::Transaction*);
      SYNCHRO_RESULT append();
      void appending();
      void clearCache();
    } // namespace datail

  } // namespace sync
} // namespace peer

#endif //IROHA_SYNCHRONIZER_H
