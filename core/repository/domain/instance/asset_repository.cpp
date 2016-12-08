//
// Created by 五十嵐太清 on 2016/12/08.
//

#ifndef IROHA_ASSET_REPOSITORY_HPP_H
#define IROHA_ASSET_REPOSITORY_HPP_H

#include <string>
#include <vector>

#include "../../world_state_repository.hpp"

namespace repository{
    namespace Asset {

        bool add(std::string publicKey,std::string assetName,std::string value){
            return world_state_repository::add(assetName+"@"+publicKey, value);
        }

        bool update(std::string publicKey,std::string assetName,std::string newValue){
            return world_state_repository::update(assetName+"@"+publicKey, newValue);
        }

        bool remove(std::string publicKey,std::string assetName){
            return world_state_repository::remove(assetName+"@"+publicKey);
        }

        std::vector <std::string> findAll(std::string key) {

        }

        std::string findOne(std::string key){

        }

        std::string findOrElse(std::string key,std::string defaultVale);

        bool isExist(std::string key) {

        }
    }
}
#endif //IROHA_ASSET_REPOSITORY_HPP_H
