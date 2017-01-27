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
#include <string>
#include <vector>

#include "../../world_state_repository.hpp"

namespace repository{
    namespace asset {

        namespace detail {
            namespace string_wrapper {
                class AssetName {
                public:
                    explicit AssetName(const std::string& assetName)
                        : _assetName(assetName) {}

                    const std::string& operator()() const {
                        return _assetName;
                    }
                    
                private:
                    std::string _assetName;
                };


                class AssetPublicKey {
                public:
                    explicit AssetPublicKey(const std::string& assetPublicKey)
                        : _assetPublicKey(assetPublicKey) {}

                    const std::string& operator()() const {
                        return _assetPublicKey;
                    }
                    
                private:
                    std::string _assetPublicKey;
                };
            }

            using namespace string_wrapper;

            inline std::string createAssetKey(
                const AssetName&      assetName,
                const AssetPublicKey& publicKey
            ) {
                return assetName() + "@" + publicKey();
            }
        }

        // TODO: Wrap std::string with structs in arguments.
        bool add(std::string publicKey, std::string assetName,std::string value) {
            return world_state_repository::add(
                detail::createAssetKey(
                    detail::AssetName(assetName),
                    detail::AssetPublicKey(publicKey)
                ),
                value
            );
        }

        // TODO: Wrap std::string with structs in arguments.
        bool update(std::string publicKey,std::string assetName,std::string newValue) {
            return world_state_repository::update(
                detail::createAssetKey(
                    detail::AssetName(assetName),
                    detail::AssetPublicKey(publicKey)
                ),
                newValue
            );
//            return world_state_repository::update(assetName+"@"+publicKey, newValue);
        }

        bool remove(std::string publicKey,std::string assetName) {
            return world_state_repository::remove(
                detail::createAssetKey(
                    detail::AssetName(assetName),
                    detail::AssetPublicKey(publicKey)
                )
            );
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
