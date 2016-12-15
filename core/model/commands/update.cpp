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

    using Object = json_parse::Object;

    template <>
    Update<Asset>::Update(
        Object obj
    ):
        Asset(obj),
        ownerPublicKey(obj.dictSub["owner"].str)
    {}

    template <>
    void Update<Asset>::execution() {
        logger::info("execution","update! Asset "+ ownerPublicKey);
        repository::asset::update(ownerPublicKey, "sample", std::to_string(value));
    }
}