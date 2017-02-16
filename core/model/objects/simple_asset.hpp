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

#ifndef IROHA_SIMPLE_ASSET_HPP
#define IROHA_SIMPLE_ASSET_HPP

#include <string>
#include <vector>
#include "base_object.hpp"

namespace object {

    class SimpleAsset {

    public:
        std::string                  name;
        std::string                domain;

        BaseObject                 object;

        std::string         contractName;

        explicit SimpleAsset(const SimpleAsset* a):
            name(a->name),
            domain(a->domain),
            object(a->object),
            contractName(a->contractName)
        {}

        explicit SimpleAsset():
            name(""),
            domain(""),
            object(),
            contractName("")
        {}


        explicit SimpleAsset(
            std::string               domain,
            std::string                 name,
            BaseObject                object,
            std::string    smartContractName
        ):
            name(name),
            domain(domain),
            object(object),
            contractName(smartContractName)
        {}

        explicit SimpleAsset(
            std::string               domain,
            std::string                 name,
            std::string    smartContractName
        ):
            name(name),
            domain(domain),
            object(),
            contractName(smartContractName)
        {}

    };

};  // namespace asset

#endif  // IROHA_SIMPLE_ASSET_HPP

