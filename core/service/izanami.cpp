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
#include <thread_pool.hpp>
#include "izanami.hpp"
#include "executor.hpp"
#include <infra/protobuf/api.pb.h>
#include <consensus/connection/connection.hpp>
#include <infra/config/peer_service_with_json.hpp>
#include <infra/config/iroha_config_with_json.hpp>
#include <crypto/hash.hpp>
//#include <repository/transaction_repository.hpp>


namespace izanami {
    using Api::TransactionResponse;

    InitializeEvent::InitializeEvent() {
        now_progress = 0;
    }

    void InitializeEvent::add_transactionResponse( std::unique_ptr<TransactionResponse> txResponse ) {
        if( now_progress > txResponse->code() ) return; // management index of progress is TransactionResponse.code()

        // make TransactionResponse hash - temporary
        std::string hash = "";
        for( auto&& tx : txResponse->transaction() ) {
            hash = hash::sha3_256_hex(hash+tx.hash());
        }
        txResponses[ hash ] = std::move( txResponse );
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
            //repository::transaction::add( hash, tx );

        }
    }
    void InitializeEvent::executeTxResponse( const std::string& hash ) {
        for( auto &&tx : txResponses[ hash ]->transaction() ) {
            executor::execute( std::move(tx) );
        }
    }
    void InitializeEvent::clear() {
        now_progress = 0;
        txResponses.clear();
        hashes.clear();
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
            if( res >= 2 * config::PeerServiceConfig::getInstance().getMaxFaulty() + 1 ) return true;
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
            if( res >= 2 * config::PeerServiceConfig::getInstance().getMaxFaulty() + 1 ) return res_hash;
            return "";
        }

        void storeTransactionResponse(InitializeEvent &event) {
            std::string hash = getCorrectHash(event);
            event.storeTxResponse(hash);
            event.executeTxResponse(hash);
            event.next_progress();
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
                event.clear();
            }
        }
    }


    static ThreadPool pool(
            ThreadPoolOptions{
                    .threads_count = config::IrohaConfigManager::getInstance()
                            .getConcurrency(0),
                    .worker_queue_size = config::IrohaConfigManager::getInstance()
                            .getPoolWorkerQueueSize(1024),
            }
    );

    //invoke when initialize Peer that to config Participation on the way
    void startIzanagi() {
        logger::explore("izanagi") <<  "\033[95m+==ーーーーーーーーーー==+\033[0m";
        logger::explore("izanagi") <<  "\033[95m|+-ーーーーーーーーーー-+|\033[0m";
        logger::explore("izanagi") <<  "\033[95m||  　　　　　　　　　 ||\033[0m";
        logger::explore("izanagi") <<  "\033[95m||初回取引履歴構築機構 ||\033[0m";
        logger::explore("izanagi") <<  "\033[95m||\033[1mいざなぎ\033[0m\033[95m　　 ||\033[0m";
        logger::explore("izanagi") <<  "\033[95m|| 　　　　　　 　　　 ||\033[0m";
        logger::explore("izanagi") <<  "\033[95m|+-ーーーーーーーーーー-+|\033[0m";
        logger::explore("izanagi") <<  "\033[95m+==ーーーーーーーーーー==+\033[0m";
        logger::explore("izanagi") <<  "- 起動/setup";

        logger::info("izanagi")    <<  "My PublicKey is " << config::PeerServiceConfig::getInstance().getMyPublicKey();
        logger::info("izanagi")    <<  "My key is " << config::PeerServiceConfig::getInstance().getMyIp();

        /*
        connection::iroha::Izanami::Transaction::receive([](const std::string& from, TransactionResponse& txResponse ) {
            logger::info("izanagi") << "receive!";
            // send processTransaction(event) as a task to processing pool
            // this returns std::future<void> object
            // (std::future).get() method locks processing until result of processTransaction will be available
            // but processTransaction returns void, so we don't have to call it and wait
            std::function<void()> &&task = std::bind(receiveTransactionResponse( std::make_unique( txResponse ) );
            pool.process(std::move(task));
        });
         */
    }

}

