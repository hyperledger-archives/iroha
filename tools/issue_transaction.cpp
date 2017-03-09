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
// Created by Takumi Yamashita on 2017/03/09.
//

#include <iostream>
#include <string>
#include <infra/config/peer_service_with_json.hpp>
#include <service/peer_service.hpp>
#include <util/logger.hpp>
#include <json.hpp>

#include "helper/issue_transaction_add.hpp"
//#include "helper/issue_transaction_contract.hpp"
#include "helper/issue_transaction_update.hpp"
#include "helper/issue_transaction_remove.hpp"
#include "helper/issue_transaction_transfer.hpp"


namespace tools {
    namespace issue_transaction {
        const std::string ADD = "add";
        const std::string TRANSFER = "transfer";
        const std::string UPDATE = "update";
        const std::string REMOVE = "remove";
        const std::string CONTRACT = "contract";

        const std::string ASSET = "asset";
        const std::string DOMAIN = "domain";
        const std::string ACCOUNT = "account";
        const std::string PEER = "peer";

        std::string tolower(std::string s) {
            for (char &c : s)
                c = tolower(c);
            return s;
        }

        void invalid_error() {
            logger::error("issue_transaction") << ("Invalid argument");
            logger::error("issue_transaction") << ("./issue_transaction [COMMAND] [DATAMODEL] [ARGS...] ");
            exit(1);
        }

        void undefined_data_error( std::string& data ) {
            logger::error("issue_transaction") << data + " is undefined data";
            exit(1);
        }



        void Add( std::string& data, std::vector<std::string>& argv ) {
            if( data == ASSET ) {

            } else if( data == DOMAIN ) {

            } else if( data == ACCOUNT ) {

            } else if( data == PEER ) {
                add::peer::issue_transaction( argv );
            } else {
                undefined_data_error( data );
            }
        }
        void Transfer( std::string& data, std::vector<std::string>& argv ) {
            if( data == ASSET ) {

            } else if( data == DOMAIN ) {

            } else if( data == ACCOUNT ) {

            } else if( data == PEER ) {

            } else {
                undefined_data_error( data );
            }
        }
        void Update( std::string& data, std::vector<std::string>& argv ) {
            if( data == ASSET ) {

            } else if( data == DOMAIN ) {

            } else if( data == ACCOUNT ) {

            } else if( data == PEER ) {

            } else {
                undefined_data_error( data );
            }
        }
        void Remove( std::string& data, std::vector<std::string>& argv ) {
            if( data == ASSET ) {

            } else if( data == DOMAIN ) {

            } else if( data == ACCOUNT ) {

            } else if( data == PEER ) {

            } else {
                undefined_data_error( data );
            }
        }
        void Contract( std::string& data, std::vector<std::string>& argv ) {
            if( data == ASSET ) {

            } else if( data == DOMAIN ) {

            } else if( data == ACCOUNT ) {

            } else if( data == PEER ) {

            } else {
                undefined_data_error( data );
            }
        }

    }
}

int main(int argc, char* argv[]){
    std::string ip = config::PeerServiceConfig::getInstance().getMyIp();
    std::string pubkey = config::PeerServiceConfig::getInstance().getMyPublicKey();
    if( argc < 3 ) {
        tools::issue_transaction::invalid_error();
    }

    // OK both lower-case or upper-case alphabet. all replace to lower-case.
    std::string command = argv[1]; command = tools::issue_transaction::tolower(command);
    std::string datamodel = argv[2]; datamodel = tools::issue_transaction::tolower(datamodel);

    std::vector<std::string> argv_s;
    for(int i=3;i<argc;i++) argv_s.emplace_back( argv[i] );

    if( command == tools::issue_transaction::ADD ) {
        tools::issue_transaction::Add( datamodel, argv_s );
    } else if( command == tools::issue_transaction::TRANSFER ) {
        tools::issue_transaction::Transfer( datamodel, argv_s );
    } else if( command == tools::issue_transaction::UPDATE ) {
        tools::issue_transaction::Update( datamodel, argv_s );
    } else if( command == tools::issue_transaction::REMOVE ) {
        tools::issue_transaction::Remove( datamodel, argv_s );
    } else if( command == tools::issue_transaction::CONTRACT ) {
        tools::issue_transaction::Contract( datamodel, argv_s );
    } else {
        logger::error("issue_transaction") << command + " is undefined command.";
        exit(1);
    }

}