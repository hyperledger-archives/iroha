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
#ifndef IROHA_CONTRACT_HPP
#define IROHA_CONTRACT_HPP

#include <string>
#include <model/objects/object.hpp>

namespace command {

    class Contract {
        std::string contractName;
        object::Object object;

        Contract(
            object::Object o,
            std::string contractName
        ):
            contractName(contractName),
            object(o)
        {}

    };

};  // namespace command


#endif //IROHA_CONTRACT_HPP
