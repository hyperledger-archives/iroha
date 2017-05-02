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

#include "validator.hpp"
#include <commands_generated.h>
#include <transaction_generated.h>
#include <ametsuchi/ametsuchi.h>
#include <ametsuchi/repository.hpp>

namespace runtime {
    namespace validator {

        bool account_exist_validator(const flatbuffers::String &publicKey){
            return repository::existAccountOf(publicKey);
        }

        auto prev_validator = [](
            const flatbuffers::String& publicKey,
            const flatbuffers::String& target_ledger,
            const flatbuffers::String& target_domain,
            const flatbuffers::String& target_asset
        ) -> std::function<bool(const ::iroha::Command)> {

            auto asset_permissions  = repository::permission::getPermissionAssetOf(publicKey);
            for(const iroha::AccountPermissionAsset* ap: asset_permissions) {
                if (
                    ap->asset_name()->str()  == target_asset.str() &&
                    ap->domain_name()->str() == target_domain.str() &&
                    ap->ledger_name()->str() == target_ledger.str()
                ){
                    return [=](const iroha::Command c) -> bool{
                        return (ap->read()  && (
                            (ap->transfer() && iroha::Command::Transfer == c ) ||
                            (ap->add()      && iroha::Command::Add == c ) ||
                            (ap->subtract() && iroha::Command::Subtract == c )
                        ));
                    };
                }
            }
            return [](const iroha::Command c) -> bool{
                return false;
            };
        };

        bool permission_validator(const iroha::Transaction& tx){
            return prev_validator(
                    *tx.creatorPubKey(),
                    *tx.command_as_AssetCreate()->ledger_name(),
                    *tx.command_as_AssetCreate()->domain_name(),
                    *tx.command_as_AssetCreate()->asset_name()
            )(tx.command_type());
        }

        bool logic_validator(const iroha::Transaction &tx){

        }

    };

};