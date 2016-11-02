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

#include "asset.hpp"

namespace asset {

    Asset::Asset(
            const std::string domain,
            const std::string name,
            unsigned long long value,
            unsigned int precision
    ):
        domain(domain),
        name(name),
        value(value),
        precision(precision)
    {}

    // WIP validation ["]
    std::string Asset::getAsJSON() {
        return
            "{\"name\":\""
            + this->name +
            "\",\"domain\":\""
            + this->domain +
            "\",\"value\":\""
            + this->value +
            "\",\"precision\":\""
            + this->precision +
            "\"}";
    }
    using Object = json_parse::Object;
    using Rule = json_parse::Rule;
    using Type = json_parse::Type;
    Object dump() {
        Object obj = Object(Type::DICT);
        obj.dictSub["name"] =  Object(Type::STR, name);
        obj.dictSub["domain"] =  Object(Type::STR, domain);
        obj.dictSub["value"] =  Object(Type::STR, value);
        obj.dictSub["precision"] =  Object(Type::STR, precision);
        return obj;
    }

    static Rule getJsonParseRule() {
        Rule obj = Rule(Type::DICT);
        obj.dictSub["name"] =  Rule(Type::STR);
        obj.dictSub["domain"] =  Rule(Type::STR);
        obj.dictSub["value"] = Rule(Type::STR);
        obj.dictSub["precision"] = Rule(Type::STR);
        return obj;
    }

};  // namespace asset
