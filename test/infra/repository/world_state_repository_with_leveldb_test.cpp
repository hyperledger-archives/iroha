//
// Created by SonokoMizuki on 2016/10/23.
//


#include "../../../core/repository/world_state_repository.hpp"

#include <leveldb/db.h>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

const std::string key = "name";
std::string value = "mizuki";


TEST(World_sate_repository_with_leveldb, addAndfind){

    repository::world_state_repository::add(key, value);

    std::string res;
    res = repository::world_state_repository::find(key);
    ASSERT_STREQ(res.c_str(), value.c_str());
}


