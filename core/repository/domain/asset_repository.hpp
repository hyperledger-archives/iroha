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
#ifndef IROHA_ASSET_REPOSITORY_H
#define IROHA_ASSET_REPOSITORY_H

#include <string>
#include <vector>

#include "../../model/objects/asset.hpp"
#include "../../model/state/asset.hpp"

namespace repository {
    namespace asset {
        
        std::string add(const std::string& domainId, const std::string& assetName, const std::string& value);
        bool update(const std::string& domainId, const std::string& assetName, const std::string& newValue);
        bool remove(const std::string& domainId, const std::string& assetName);
        std::vector <object::Asset> findAll(const std::string& key);

        object::Asset findByUuid(const std::string& uuid);
        
        object::Asset findByUuidOrElse(const std::string& uuid, const object::Asset& defaultValue);

        bool isExist(const std::string& key);
    };
};


#endif //IROHA_ASSET_REPOSITORY_H
