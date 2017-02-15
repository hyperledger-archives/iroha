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
#include "base_object.hpp"

template <>
constexpr int get<int>(){
    if(type != Type::INTEGER){
        throw except("This base object type is not integer.",__FILE__);
    }
    return integer;
}

template <>
constexpr char* get<char*>(){
    if(type != Type::TEXT){
        throw except("This base object type is not text.",__FILE__);
    }
    return text;
}

template <>
constexpr bool get<bool>(){
    if(type != Type::BOOLEAN){
        throw except("This base object type is not boolean.",__FILE__);
    }
    return boolean;
}

template <>
constexpr float get<float>(){
    if(type != Type::DECIMAL){
        throw except("This base object type is not decimal.",__FILE__);
    }
    return decimal;
}