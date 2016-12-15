/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp

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
#ifndef IROHA_UPDATE_H
#define IROHA_UPDATE_H

#include "../objects/domain.hpp"
#include "../objects/asset.hpp"
#include "../objects/message.hpp"

#include <string>
#include <iostream>
#include "../../util/logger.hpp"
#include "../../repository/domain/instance/asset_repository.hpp"
#include "../../service/json_parse.hpp"

namespace command {

    template <typename T>
    class Update: protected T {
    protected:
        std::string ownerPublicKey;
    public:


        template<typename... Args>
        Update(
            std::string&& ownerPublicKey,
            Args&&... args
        ):
            ownerPublicKey(ownerPublicKey),
            T(std::forward<Args>(args)...)
        {}

        std::string getCommandName() const{
            return "Update";
        }

        using Rule = json_parse::Rule;
        using Type = json_parse::Type;
        using Object = json_parse::Object;

        Update(
            Object obj
        );

        void execution();

        Object dump() {
            auto obj = Object(Type::DICT);
            obj.dictSub.insert( std::make_pair( "name",   Object(Type::STR, getCommandName())));
            obj.dictSub.insert( std::make_pair( "object", T::dump()));
            obj.dictSub.insert( std::make_pair( "owner",  Object(Type::STR, ownerPublicKey)));
            return obj;
        }

        static Rule getJsonParseRule() {
            auto rule = Rule(Type::DICT);
            rule.dictSub.insert( std::make_pair( "name",   Rule(Type::STR)));
            rule.dictSub.insert( std::make_pair( "object", T::getJsonParseRule()));
            rule.dictSub.insert( std::make_pair( "owner",  Rule(Type::STR)));
            return rule;
        }
    };

};  // namespace command

#endif //IROHA_UPDATE_H
