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

#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "base_object.hpp"
#include "../../util/logger.hpp"

namespace object {

    BaseObject::BaseObject() = default;	// ctor for BaseObjectValueT::null

    BaseObject::BaseObject(BooleanT rhs) noexcept : boolean(rhs) {}
    BaseObject::BaseObject(IntegerT rhs) noexcept : integer(rhs) {}
    BaseObject::BaseObject(DecimalT rhs) noexcept : decimal(rhs) {}

    BaseObject::BaseObject(ValueT t) {
        switch (t) {              

            case BaseObjectValueT::array: {
                array = detail::allocateObject<ArrayObjectT>();
                break;
            }

            case BaseObjectValueT::string: {
                text = detail::allocateObject<StringT>("");
                break;
            }

            case BaseObjectValueT::boolean: {
                boolean = BooleanT(false);
                break;
            }

            case BaseObjectValueT::integer: {
                integer = IntegerT(0);
                break;
            }

            case BaseObjectValueT::decimal: {
                decimal = DecimalT(0.0);
                break;
            }

            case BaseObjectValueT::null: {
            	break;
            }

            default: {
               	logger::fatal("model object") << "Unexpected ValueT: " << static_cast<std::uint8_t>(t);
               	exit(EXIT_FAILURE);
                break;
            }
        }
    }

    BaseObject::BaseObject(const StringT& rhs) {
        text = detail::allocateObject<StringT>(rhs);
    }

    BaseObject::BaseObject(const ArrayObjectT& rhs) {
        array = detail::allocateObject<ArrayObjectT>(rhs);
    }
}
