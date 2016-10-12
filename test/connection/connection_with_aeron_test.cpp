#include "../../core/consensus/connection/connection.hpp"

#include <string>
#include <iostream>

#include <unordered_map>

int main(int argc, char* argv[]){
    if(argc < 2){
        return 1;
    }
    std::unordered_map<std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>{"address","45.32.152.218"});
    config.insert(std::pair<std::string, std::string>{"port","1234"});
    config.insert(std::pair<std::string, std::string>{"subscribeStreamId","1234"});
    config.insert(std::pair<std::string, std::string>{"publishStreamId","1235"});
    config.insert(std::pair<std::string, std::string>{"subscribeChannel","1234"});
    config.insert(std::pair<std::string, std::string>{"publishChannel","1235"});

    connection::initialize_peer(config);
while(1){
    if(std::string(argv[1]) == "sender"){
        if(argc != 3){
            return 1;
        }
        connection::sendAll("Mizuki < nya-n!");
    }else{
        connection::receive([](std::string from, std::string message){
            std::cout <<" receive :" << message <<" from:"<< from << "\n";
        });
    }
}
    return 0;
}
