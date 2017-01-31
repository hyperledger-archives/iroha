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
#include "../../../model/string_wrapper/string_wrapper.hpp"

namespace repository{
    namespace asset {

        using string_wrapper::DomainId;
        using string_wrapper::AssetName;

        // TODO: replace map<string,Object>
        using AssetValue = std::string;

        namespace detail {
            inline std::string serializeAsset(const object::Asset& obj) {
                Event::Asset protoAsset = convertor::detail::encodeObject(obj);
                std::string ret;
                protoAsset.SerializeToString(&ret);
                return ret;
            }

            inline object::Asset deserializeAsset(const std::string& serializedAsset) {
                Event::Asset protoAsset;
                protoAsset.ParseFromString(serializedAsset);
                return convertor::detail::decodeObject(protoAsset);
            }

            // TODO: replace with util::createUuid() ?
            inline std::string createAssetUuid(const DomainId& domainId, const AssetName& assetName) {
                return hash::sha3_256_hex(*domainId + "@" + *assetName);
            }
        }

        // TODO: use optional
        std::string add(const std::string& domainId, const std::string& assetName, const std::string& value) {

            logger::explore("asset repository") << "domainId: " << domainId << " assetName: " << assetName << " assetValue: " << value;

            const auto uuid = detail::createAssetUuid(
                DomainId(domainId),
                AssetName(assetName)
            );

            const auto serializedAsset = detail::serializeAsset(
                object::Asset(domainId, assetName, std::stoi(value)/*value*/)
            );

            if (not world_state_repository::add(uuid, serializedAsset)) {
                return "";
            }

            return uuid;
        }

        // TODO: Wrap std::string with structs in arguments.
        bool update(const std::string& domainId, const std::string& assetName, const std::string& newValue) {

            const auto uuid = detail::createAssetUuid(
                DomainId(domainId),
                AssetName(assetName)
            );

            const auto serializedAsset = detail::serializeAsset(object::Asset(domainId, assetName, std::stoull(newValue)/*value*/));

            return world_state_repository::update(uuid, newValue);
        }

        bool remove(const std::string& domainId, const std::string& assetName) {

            const auto uuid = detail::createAssetUuid(
                DomainId(domainId),
                AssetName(assetName)
            );

            return world_state_repository::remove(uuid);
        }
        
        std::vector <object::Asset> findAll(const std::string& uuid) {
            
        }

        // TODO: use optional type
        object::Asset findByUuid(const std::string& uuid) {

            const std::string serializedAsset = world_state_repository::find(uuid);

            logger::debug("asset repository :: findByUuid") << serializedAsset;

            if (serializedAsset.empty()) {
                return object::Asset();
            }

            return detail::deserializeAsset(serializedAsset);
        }

        object::Asset findByUuidOrElse(const std::string& uuid, const object::Asset& defaultValue) {
            throw "asset repo :: findByUuidOrElse() is not implemented yet.";
        }

        bool isExist(const std::string& key) {
            throw "asset repo :: isExist() is not implemented yet.";
        }
    }
}
