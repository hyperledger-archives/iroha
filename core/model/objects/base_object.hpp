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
#include <string>

struct BaseObject {

    enum class Type{
        INTEGER,
        TEXT,
        BOOLEAN,
        DECIMAL,
        NONE
    };


    Type type;

    int             integer;
    std::string        text;
    bool            boolean;
    float           decimal;

    explicit BaseObject():type(Type::NONE),
       integer(0),text(""),boolean(false),decimal(0.0f){}

    explicit BaseObject(int           i):type(Type::INTEGER),
       integer(i),text(""),  boolean(false),decimal(0.0f){}
    explicit BaseObject(std::string   t):type(Type::TEXT   ),
       integer(0),text(t),boolean(false),decimal(0.0f){}
    explicit BaseObject(bool          b):type(Type::BOOLEAN),
       integer(0),text(""),boolean(b),decimal(0.0f){}
    explicit BaseObject(float         d):type(Type::DECIMAL),
       integer(0),text(""),boolean(false),decimal(d){}


    using except = exception::InvalidCastException;

    template <typename T>
    const T get(){}

};
#endif //IROHA_BASE_OBJECT_HPP
