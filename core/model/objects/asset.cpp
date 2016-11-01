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
#include "../Transaction.hpp"

namespace asset {

    Asset::Asset(
            const std::string &domain,
            const std::string &name,
            unsigned long long value;
            unsigned int precision;
    ):
        domain(domain),
        asset(asset),
        value(value),
        precision(precision)
    {}

    std::string Transaction::getAsJSON() const {
        return "{\"name\":\""
               + name +
                "\",\"domain\":\""
               + domain +
                "\",\"value\":\""
                + value +
                "\",\"precision\":\""
                + precision +
                "\"}";
    }

};  // namespace asset
