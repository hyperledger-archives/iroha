//
// Created by SonokoMizuki on 2016/11/01.
//

#ifndef IROHA_OBJECTS_H
#define IROHA_OBJECTS_H

#include "../../service/json_parse.hpp"

namespace object{
    // Abstract
    class BaseObject {
        int64_t integer;
        double  decimal;
        std::string text;
        bool    boolean;        
    };
}

#endif //IROHA_OBJECTS_H
