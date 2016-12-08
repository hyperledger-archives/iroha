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
#include "update.hpp"

namespace command {

    using object::Asset;

    template <>
    Update<Asset>::Update(
        const std::string& ownerPublicKey,
        const std::string& name,
        const unsigned long long& value
    ):
        Asset(name,value),
        ownerPublicKey(ownerPublicKey)
    {}

    using Object = json_parse::Object;

    template <>
    Update<Asset>::Update(
        Object obj
    ):
        Asset(obj),
        ownerPublicKey(obj.dictSub["owner"].str)
    {}

}