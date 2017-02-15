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

#include "../../util/exception.hpp"

struct BaseObject {

    enum class Type{
        INTEGER,
        TEXT,
        BOOLEAN,
        DECIMAL
    };

    int             integer;
    char*              text;
    bool            boolean;
    float           decimal;
    Type type;

    constexpr explicit BaseObject(int     i):type(Type::INTEGER){ integer = i;}
    constexpr explicit BaseObject(char*   t):type(Type::TEXT   ){ text    = t;}
    constexpr explicit BaseObject(bool    b):type(Type::BOOLEAN){ boolean = b;}
    constexpr explicit BaseObject(float   d):type(Type::DECIMAL){ decimal = d;}

    ~BaseObject(){
        if(type == Type::TEXT){
            delete text;
        }
    }

    using except = exception::InvalidCastException;

    template <typename T>
    constexpr T get(){
    }

    constexpr get(){
        if(type != Type::INTEGER){
            throw except("This base object type is not integer.",__FILE__);
        }
        return integer;
    }


};
#endif //IROHA_BASE_OBJECT_HPP
