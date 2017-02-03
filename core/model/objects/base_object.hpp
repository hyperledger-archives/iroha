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

#ifndef IROHA_BASE_OBJECT_HPP
#define IROHA_BASE_OBJECT_HPP

#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "../../util/logger.hpp"

namespace object {

    enum class BaseObjectValueT : std::uint8_t {
        null,
        object,
        array,
        string,
        boolean,
        integer,
        decimal
    };

    using ObjectT   = union Object;
    template<class T> using ArrayT = std::vector<T>;
    using ArrayObjectT = ArrayT<ObjectT*>;
    using StringT   = std::string;
    using BooleanT  = bool;
    using IntegerT  = std::int64_t;
    using DecimalT  = double;

    union BaseObject {
        ArrayObjectT*	array;
        StringT*        text;
        BooleanT        boolean;
        IntegerT        integer;
        DecimalT        decimal;

        BaseObject();

        BaseObject(BooleanT rhs) noexcept;
        BaseObject(IntegerT rhs) noexcept;
        BaseObject(DecimalT rhs) noexcept;
        BaseObject(BaseObjectValueT t);
        BaseObject(const StringT& rhs);
        BaseObject(const ArrayObjectT& rhs);
    };
}

#endif // IROHA_BASE_OBJECT_HPP
