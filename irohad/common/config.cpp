/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#ifndef __RANDOM_HPP_
#define __RANDOM_HPP_

#include <common/config.hpp>
#include <type_traits>

#include <fstream>
#include <random>

namespace common {
  namespace config {
    using namespace rapidjson;

    ConfigLoader::ConfigLoader(const std::string& file_name) {
      std::ifstream ifs(file_name);

      if (not ifs.is_open()) {
        // TODO: exit with error msg
      }

      IStreamWrapper isw(ifs);
      doc.ParseStream(isw);

      if (doc.HasParseError()) {
        // ToDo log failed to logger or maybe just exit with error?
        doc.Parse("{}");
      }
    }

    int ConfigLoader::getIntOrDefault(const std::string& key, int def) {
      if (doc.IsObject() && doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt()){
        return doc[key.c_str()].GetInt();
      } else {
        return def;
      };
    }

    std::string ConfigLoader::getStringOrDefault(const std::string& key, const std::string &def) {
      if (doc.IsObject() && doc.HasMember(key.c_str()) && doc[key.c_str()].IsString()) {
        return doc[key.c_str()].GetString();
      } else {
        return def;
      };
    }

    bool ConfigLoader::getBoolOrDefault(const std::string& key, bool def) {
      if (doc.IsObject() && doc.HasMember(key.c_str()) && doc[key.c_str()].IsBool()) {
        return doc[key.c_str()].GetBool();
      } else {
        return def;
      };
    }

  };  // namepsace config
};    // namespace common

#endif
