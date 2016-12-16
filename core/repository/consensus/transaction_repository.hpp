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

#ifndef IROHA_TRANSACTION_REPOSITORY_HPP
#define IROHA_TRANSACTION_REPOSITORY_HPP

#include "../../model/transaction.hpp"
#include "../../infra/protobuf/convertor.hpp"

namespace repository{
    namespace transaction {

        void add(std::string& key,std::string value);

        std::vector<std::string> findAll();

        std::vector<std::string> findByAssetName(std::string name);

    }
}


#endif //IROHA_TRANSACTION_REPOSITORY_HPP
