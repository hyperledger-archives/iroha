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

#include <cstdint>
#include <infra/protobuf/convertor.hpp>
#include <model/state/account.hpp>
#include <model/string_wrapper/string_wrapper.hpp>
#include <repository/world_state_repository.hpp>
#include "../account_repository.hpp"

namespace repository{
    namespace account {

        using string_wrapper::PublicKey;
        using string_wrapper::Alias;

        namespace detail {
            std::string serializeAccount(const object::Account& obj) {
                Event::Account protoAccount = convertor::detail::encodeObject(obj);
                std::string ret;
                protoAccount.SerializeToString(&ret);
                return ret;
            }

            object::Account deserializeAccount(const std::string& str) {
                Event::Account protoAccount;
                protoAccount.ParseFromString(str);
                return convertor::detail::decodeObject(protoAccount);
            }

            // TODO: replace with util::createUuid() ?
            std::string createAccountUuid(const PublicKey& publicKey) {
                return hash::sha3_256_hex(*publicKey);
            }
        }

        // TODO: use string_wrapper
        std::string add(const std::string& publicKey, const std::string& alias) {

            logger::explore("account repository") << "Add publicKey:" << publicKey << " alias:" << alias;

            const auto uuid = detail::createAccountUuid(PublicKey(publicKey));
            const auto serializedAccount = detail::serializeAccount(object::Account(publicKey, alias));

            logger::debug("account repository") << "Save key:" << uuid << " alias:" << alias;

            if (not world_state_repository::add(uuid, serializedAccount)) {
                return "";
            }

            return uuid;
        }

        // SampleAsset has only quantity no logic, so this value is int.
        bool update_quantity(
            const std::string&  uuid,
            const std::string&  assetName,
            std::int64_t        newValue
        ) {

            const auto serializedAccount  = world_state_repository::find(uuid);
            
            object::Account account = detail::deserializeAccount(serializedAccount);
            for (auto& asset: account.assets) {
                if (std::get<0>(asset) == assetName) {  // asset.name == assetName (can adapt struct?)
                    std::get<1>(asset) = newValue;      // asset.value = newValue
                }
            }

            return world_state_repository::update(uuid, detail::serializeAccount(account));
        }

        bool attach(const std::string& uuid, const std::string& assetName, std::int64_t assetDefault) {

            const auto serializedAccount = world_state_repository::find(uuid);

            if (serializedAccount.empty()) {
                return false;
            }

            object::Account account = detail::deserializeAccount(serializedAccount);
            account.assets.emplace_back(assetName, assetDefault);

            return world_state_repository::update(uuid, detail::serializeAccount(account));
        }

        object::Account findByUuid(const std::string& uuid) {

            const auto serializedAccount = world_state_repository::find(uuid);

            logger::debug("account repository :: findByUuid") << serializedAccount;

            if (serializedAccount.empty()) {
                return object::Account();
            }

            return detail::deserializeAccount(serializedAccount);
        }

    }
}
