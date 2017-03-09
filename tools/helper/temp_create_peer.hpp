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
// Created by Takumi Yamashita on 2017/03/10.
//

#include <iostream>
#include <vector>
#include <stdexcept>
#include <util/logger.hpp>
#include <transaction_builder/transaction_builder.hpp>
#include <consensus/connection/connection.hpp>
#include <infra/config/peer_service_with_json.hpp>

#define TOOLS_ISSUE_TRANSACTION_ADD_HPP
#ifndef TOOLS_ISSUE_TRANSACTION_ADD_HPP

namespace tools {
    namespace issue_transaction {
        namespace add {
            namespace account {
                void issue_transaction( std::vector<std::string>& argv ) {
                    auto tx = txbuilder::TransactionBuilder<Add<Account>>()
                            .setSenderPublicKey(config::PeerServiceConfig::getMyPublicKey())
                            .setAccount(
//TODO
                            )
                            .build();
                    connection::iroha::PeerService::Sumeragi::send(
                            config::PeerServiceConfig::getInstance().getMyIp(),
                            tx
                    )
                } catch (const out_of_range& oor) {
                    logger::error("issue_transaction") << "Not enough elements." << endl;
                    logger::error("issue_transaction") << oor.what() << endl;
                    exit(1);
                }
            }
            namespace asset {
                void issue_transaction( std::vector<std::string>& argv ) {
                    auto tx = txbuilder::TransactionBuilder<Add<Asset>>()
                            .setSenderPublicKey(config::PeerServiceConfig::getMyPublicKey())
                            .setAsset(
//TODO
                            )
                            .build();
                    connection::iroha::PeerService::Sumeragi::send(
                            config::PeerServiceConfig::getInstance().getMyIp(),
                            tx
                    )
                } catch (const out_of_range& oor) {
                    logger::error("issue_transaction") << "Not enough elements." << endl;
                    logger::error("issue_transaction") << oor.what() << endl;
                    exit(1);
                }
            }
            namespace peer {
                void issue_transaction( std::vector<std::string>& argv ) {
                    auto tx = txbuilder::TransactionBuilder<Add<Peer>>()
                            .setSenderPublicKey(config::PeerServiceConfig::getMyPublicKey())
                            .setPeer(
//TODO
                            )
                            .build();
                    connection::iroha::PeerService::Sumeragi::send(
                            config::PeerServiceConfig::getInstance().getMyIp(),
                            tx
                    )
                } catch (const out_of_range& oor) {
                    logger::error("issue_transaction") << "Not enough elements." << endl;
                    logger::error("issue_transaction") << oor.what() << endl;
                    exit(1);
                }
            }
            }
        }
    }
}