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
#include "add.hpp"

#include "../objects/account.hpp"
#include "../objects/asset.hpp"
#include "../../util/logger.hpp"

namespace command {

    auto default_currency = "iroha";

    template <>
    void Add<object::Account>::execution() {
        auto uuid = repository::account::add( publicKey, name);
        repository::account::attach(uuid, default_currency, 100);
    }

    template <>
    void Add<object::Asset>::execution() {
        auto pubkey = "aa";
        repository::asset::add( "creator", name, std::to_string(value));
    }

}
