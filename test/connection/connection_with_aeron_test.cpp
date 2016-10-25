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

#include "../../core/consensus/connection/connection.hpp"

#include <string>
#include <iostream>

#include <unordered_map>

int main(int argc, char* argv[]){
    if(argc != 3){
        return 1;
    }

    connection::initialize_peer(nullptr);

    if(std::string(argv[1]) == "sender"){
        connection::addPublication(argv[2]);
        while(1){
            connection::sendAll("Mizuki < nya-n!");
        }
    }else{
        connection::exec_subscription(argv[2]);
        connection::receive([](std::string from, std::string message){
            std::cout <<" receive :" << message <<" from:"<< from << "\n";
        });
    }
    while(1){}
    connection::finish();
    return 0;
}
