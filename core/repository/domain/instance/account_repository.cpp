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

#include "../../../infra/protobuf/convertor.hpp"

#include "../../../model/state/account.hpp"

#include "../account_repository.hpp"
#include "../../world_state_repository.hpp"

namespace repository{
    namespace account {

        namespace detail {
            // TODO: Separate string_wrapper to header for wrapper.
            namespace string_wrapper {
                class AccountName {
                public:
                    explicit AccountName(const std::string& accountName)
                        : _accountName(accountName) {}

                    const std::string& operator()() const {
                        return _accountName;
                    }
                    
                private:
                    std::string _accountName;
                };

                class PublicKey {
                public:
                    explicit PublicKey(const std::string& publicKey)
                        : _publicKey(publicKey) {}

                    const std::string& operator()() const {
                        return _publicKey;
                    }
                    
                private:
                    std::string _publicKey;
                };

                class Alias {
                public:
                    explicit Alias(const std::string& alias)
                        : _alias(alias) {}

                    const std::string& operator()() const {
                        return _alias;
                    }
                    
                private:
                    std::string _alias;
                };
            }
            
            using namespace string_wrapper;
            
            std::string serializeAccount(const PublicKey& publicKey, const Alias& alias) {
                auto protoAccount = convertor::detail::encodeObject(
                    object::Account(publicKey(), alias()) // object::Account(publicKey, alias)
                );
                std::string strAccount;
                protoAccount.SerializeToString(&strAccount);
                return strAccount;
            }

            std::string createAccountUuid(const PublicKey& publicKey) {
                return hash::sha3_256_hex(publicKey());
            }
        }

        // SampleAsset has only quantity no logic, so this value is int.
        bool update_quantity(
            const std::string&  uuid,
            const std::string&  assetName,
            std::int64_t        newValue
        ){
            auto account = world_state_repository::find(uuid);
            Event::Account protoAccount;
            protoAccount.ParseFromString(account);

            for (int i = 0;i < protoAccount.assets_size(); i++) {
                if (protoAccount.assets(i).name() == assetName) {
                    //protoAccount.mutable_assets(i)->set_value(newValue);
                }
            }

            std::string strAccount;
            protoAccount.SerializeToString(&strAccount);
            world_state_repository::update(uuid, strAccount);
        }

        bool attach(const std::string& uuid, const std::string& assetName, long assetDefault) {
            auto serializedAccount = world_state_repository::find(uuid);
            if(serializedAccount != "") {
                Event::Account protoAccount;
                protoAccount.ParseFromString(serializedAccount);
                auto account = convertor::detail::decodeObject(protoAccount);
                account.assets.push_back(std::make_pair(assetName,assetDefault));
                auto protoAccountNew = convertor::detail::encodeObject(account);

                std::string strAccount;
                protoAccountNew.SerializeToString(&strAccount);
                return world_state_repository::update(uuid, strAccount);
            } else {
                return false;
            }
        }

        object::Account findByUuid(const std::string& uuid) {

            const auto serializedAccount = world_state_repository::find(uuid);

            logger::debug("account repository :: findByUuid")
                << serializedAccount;

            if (serializedAccount.empty()) {
                return object::Account();
            }

            Event::Account protoAccount;
            protoAccount.ParseFromString(serializedAccount);

            return convertor::detail::decodeObject(protoAccount);
        }

        std::string add(const std::string& publicKey, const std::string& alias) {
            logger::explore("account repository")
                << "Add publicKey:" << publicKey << " alias:" << alias;

            const auto uuid = detail::createAccountUuid(detail::PublicKey(publicKey));

            const auto serializedAccount = detail::serializeAccount(
                detail::PublicKey(publicKey),
                detail::Alias(alias)
            );

            logger::debug("account repository") << "Save key:" << uuid << " alias:" << alias;

            if (not world_state_repository::add(uuid, serializedAccount)) {
                return "";
            }
            return uuid;
        }

    };
};
