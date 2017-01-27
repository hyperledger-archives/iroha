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

#include "../../../infra/protobuf/convertor.hpp"
#include "../../world_state_repository.hpp"
#include "../../../model/state/asset.hpp"
#include "../asset_repository.hpp"

namespace repository{
    namespace asset {

        namespace detail {

            // This should be placed elsewhere. (in model?)
            namespace string_wrapper {

                // duplicate (account)
                class PublicKey {
                public:
                    explicit PublicKey(const std::string& uuid)
                        : _publicKey(uuid) {}

                    const std::string& operator()() const {
                        return _publicKey;
                    }

                private:
                    std::string _publicKey;
                };

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
            }

            using namespace string_wrapper;

            using AssetValue = std::string; // TODO: replace map<string,Object>

//            inline std::string createAssetDBKey(const AssetName& assetName, const DomainId& parentDomainId) {
            inline std::string createAssetUuid(const PublicKey& publicKey, const AssetName& assetName) {
                // I think the following coding should be same as repository::account.
//                auto protoAsset = convertor::detail::encodeObject(object::Asset(...)); // name, parentDomainId
//                std::string strAsset;
//                protoAsset.SerializeToString(&strAsset);
//                return strAsset;
                return publicKey() + "@" + assetName();
            }
        }

        std::string add(std::string publicKey, std::string assetName, std::string value) {

            const auto uuid = detail::createAssetUuid(
                detail::PublicKey(publicKey),
                detail::AssetName(assetName)
            );

            if (not world_state_repository::add(uuid, value)) {
                return "";
            }

            return uuid;
        }

        // TODO: Wrap std::string with structs in arguments.
        bool update(std::string publicKey, std::string assetName, std::string newValue) {

            const auto uuid = detail::createAssetUuid(
                detail::PublicKey(publicKey),
                detail::AssetName(assetName)
            );

            return world_state_repository::update(uuid, newValue);
        }

        bool remove(std::string publicKey, std::string assetName) {

            const auto uuid = detail::createAssetUuid(
                detail::PublicKey(publicKey),
                detail::AssetName(assetName)
            );

            return world_state_repository::remove(uuid);
        }
        
        std::vector <std::string> findAll(std::string key) {
            
        }

        object::Asset findByUuid(const std::string& uuid) {

            const auto serializedAsset = world_state_repository::find(uuid);

            logger::debug("asset repository :: findByUuid") << serializedAsset;

            if (serializedAsset.empty()) {
                return object::Asset();
            }

            Event::Asset protoAsset;
            protoAsset.ParseFromString(serializedAsset);

            return convertor::detail::decodeObject(protoAsset);
        }

        std::string findOrElse(std::string key, std::string defaultValue);

        bool isExist(std::string key) {

        }
    }
}
