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

#include <vector>
#include <string>
#include <memory>
#include "izanami.hpp"
#include <infra/protobuf/api.pb.h>
#include <infra/config/peer_service_with_json.hpp>

namespace izanami {
    using Api::TransactionResponse;

    //uint64_t InitializeEvent::now_progress;
    //std::unordered_map <std::string, std::unique_ptr<TransactionResponse> > InitializeEvent::txResponses;
    //std::unordered_map <uint64_t, std::vector<std::string>> InitializeEvent::hashes;

    void InitializeEvent::add_transactionResponse( std::unique_ptr<TransactionResponse> ) {
        //if( now_progress > txResponse.getProgress() ) return; TODO getProgress() is hasn't  txResponse
        //std::string hash = txResponse.getHash(); TODO getHash is hasn't txResponse
        //txResponses[ hash ] = std::move( txResponse );
    }
    const std::vector<std::string>& InitializeEvent::getHashes( uint64_t progress ) {
        return hashes[ progress ];
    }
    const std::unique_ptr<TransactionResponse> InitializeEvent::getTransactionResponse( const std::string& hash ) {
        return std::move( txResponses[ hash ] );
    }
    void InitializeEvent::next_progress() {
        for( auto&& hash : hashes[now_progress] ) {
            txResponses.erase( hash );
        }
        hashes.erase( now_progress++ );
    }
    uint64_t InitializeEvent::now() const {
        return now_progress;
    }

    void InitializeEvent::storeTxResponse( const std::string& hash ) {
        for( auto &&tx : txResponses[ hash ]->transaction() ) {
            // TODO store txResponses[hash] to DB
        }
    }
    void InitializeEvent::executeTxResponse( const std::string& hash ) {
        for( auto &&tx : txResponses[ hash ]->transaction() ) {
            // TODO execute tx ( transaction )
        }
    }

    namespace detail {
        bool isFinishedReceiveAll(InitializeEvent &event) {
            return true; // TODO Don't underestand all Receive Finished.
        }

        bool isFinishedReceive(InitializeEvent &event) {
            std::unordered_map<std::string, int> hash_counter;
            for (std::string hash : event.getHashes(event.now())) {
                hash_counter[hash]++;
            }
            int res = 0;
            for (auto counter : hash_counter ) {
                res = std::max(res, counter.second);
            }
            //        if( res >= 2 * f + 1 ) return true; TODO f is don't decined.
            return false;
        }

        std::string getCorrectHash(InitializeEvent &event) {
            std::unordered_map<std::string, int> hash_counter;
            for (std::string hash : event.getHashes(event.now())) {
                hash_counter[hash]++;
            }
            int res = 0;
            std::string res_hash;
            for (auto counter : hash_counter ) {
                if (res < counter.second) {
                    res = counter.second;
                    res_hash = counter.first;
                }
            }
            //        if( res >= 2 * f + 1 ) return res_hash; TODO f is don't decined.
            return "";
        }

        void storeTransactionResponse(InitializeEvent &event) {
            std::string hash = getCorrectHash(event);
            // TODO store txResponse to DB
//            event.storeTxResponse(hash);
            // TODO execute txReponse
//            event.executeTxResponse(hash);
        }
    }

    //invoke when receive TransactionResponse.
    void receiveTransactionResponse( std::unique_ptr<TransactionResponse> txResponse ) {
        static InitializeEvent event;
        event.add_transactionResponse( std::move(txResponse) );
        if( detail::isFinishedReceive( event ) ) {
            detail::storeTransactionResponse( event );
            if( detail::isFinishedReceiveAll( event ) ) {
                config::PeerServiceConfig::getInstance().finishedInitializePeer();
            }
        }
    }

}

