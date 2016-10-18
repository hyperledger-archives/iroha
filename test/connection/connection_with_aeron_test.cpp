#include "../../core/consensus/connection/connection.hpp"

#include <string>
#include <iostream>

#include <unordered_map>

int main(int argc, char* argv[]){
    if(argc < 2){
        return 1;
    }
    
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
