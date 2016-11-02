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

#include "domain.hpp"

namespace domain {

    Domain::Domain(
            std::string ownerPublicKey,
            std::string name
    ):
        ownerPublicKey(ownerPublicKey),
        name(name)
    {}

    std::string Domain::getAsJSON(){
        return
            "{\"name\":\""
            + this->name +
            "\",\"ownerPublicKey\":\""
            + this->ownerPublicKey +
            "\"}";
    }
    using Object = json_parse::Object;
    using Rule = json_parse::Rule;
    using Type = json_parse::Type;
    Object dump() {
        Object obj = Object(Type::DICT);
        obj.dictSub["ownerPublicKey"] =  Object(Type::STR, ownerPublicKey);
        obj.dictSub["name"] =  Object(Type::STR, name);
        return obj;
    }

    static Rule getJsonParseRule() {
        Rule obj = Rule(ype::DICT);
        obj.dictSub["ownerPublicKey"] =  Rule(Type::STR);
        obj.dictSub["name"] = Rule(Type::STR);
        return obj;
    }

};  // namespace domain
