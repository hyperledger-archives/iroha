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
#include <infra/protobuf/api.pb.h>
#include <infra/config/peer_service_with_json.hpp>

namespace izanami {
    using Api::TransactionResponse;


    class InitializeEvent {
    private:
        uint64_t now_progress;
        unordered_map <std::string, std::unique_ptr<TransactionResponse> > txReponses;
        unordered_map <uint64_t, std::vector<std::string>> hashes;
    public:

        void add_transactionResponse( unique_ptr<TransactionResponse> txResponse) {
            //if( now_progress > txResponse.getProgress() ) return; TODO getProgress() is hasn't  txResponse
            //std::string hash = txResponse.getHash(); TODO getHash is hasn't txResponse
            txResponses[ hash ] = std::move( txResponse );
        }
        const std::vector<std::string>& getHashes( uint64_t progress ) {
            return hashes[ progress ];
        }
        const std::unique_ptr<TransactioResponse> getTransactionResponse( const std::string& hash ) {
            return std::move( txReponses[ hash ] );
        }
        void next_progress() {
            for( const auto& hash : hashes[now_progress] ) {
                txReponses.erase( hash );
            }
            hash.erase( now_progress++ );
        }
        uint64_t now() const {
            return now_progress;
        }
    };

    namespace detail {
        bool isFinishedReceiveAll(InitializeEvent &event) {
            return true; // TODO Don't underestand all Receive Finished.
        }

        bool isFinishedReceive(InitializeEvent &event) {
            unordered_map<std::string, int> hash_counter;
            for (auto hash : event.getHashes(event.now())) {
                hash_counter[hash]++;
            }
            int res = 0;
            if (const auto &counter : hash_counter ){
                res = max(res, counter.second);
            }
            //        if( res >= 2 * f + 1 ) return true; TODO f is don't decined.
            return false;
        }

        std::string getCorrectHash(InitializeEvent &event) {
            unordered_map<std::string, int> hash_counter;
            for (const std::string &hash : event.getHashes(event.now())) {
                hash_counter[hash]++;
            }
            int res = 0;
            std::string res_hash;
            if (const auto &counter : hash_counter ){
                if (res < counter.second) {
                    res = counter.second;
                    res_hash = counter.first;
                }
            }
            //        if( res >= 2 * f + 1 ) return res_hash; TODO f is don't decined.
            return "";
        }

        void storeTransactionResponse(InitializeEvent &event) {
            std::string hash = getCorrectHash();
            auto txResponse = event.getTransactionResponse(hash);
            // TODO store txResponse to DB
        }
    }

    //invoke when receive TransactionResponse.
    void receiveTransactionResponse( std::unique_ptr<TransactionResponse> txResponse ) {
        static InitializeEvent event;
        event.add_transactionResponse( txResponse );
        if( detail::isFinishedReceive( event ) ) {
            detail::storeTransactionResponse( event );
            if( detail::isFinishedReveiveAll( event ) ) {
                config::PeerServiceConfig::getInstance().finishedInitializePeer();
            }
        }
    }

}

