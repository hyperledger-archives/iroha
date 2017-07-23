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

#ifndef __COMMON_CONFIG_HPP_
#define __COMMON_CONFIG_HPP_

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace common {
  namespace config {

    using namespace rapidjson;

    class ConfigLoader {
     public:
      ConfigLoader(const std::string& file);

      int getIntOrDefault(const std::string& key, int def);
      std::string getStringOrDefault(const std::string& key, const std::string& def);
      bool getBoolOrDefault(const std::string& key, bool def);

      // template <class T>
      // T getValueOrDefault(const std::string& key, T&& def) {
      //   const auto& val = doc[key.c_str()];
      //   if (!(doc.IsObject() && doc.HasMember(key.c_str()))) {
      //     goto end;
      //   }

      //   if (std::is_same<T, int>::value && val.IsInt()) {
      //     // quite dirty hack for pleasing the compiler
      //     // maybe that switch(type){..} possible
      //     // to make compile-time
      //     auto v = val.GetInt();
      //     return *((T*)(&v));
      //   }

      //   if (std::is_same<T, std::string>::value && val.IsString()) {
      //     auto v = val.GetString();
      //     return *((T*)(&v));
      //   }

      //   if (std::is_same<T, bool>::value && val.IsBool()) {
      //     auto v = val.GetBool();
      //     return *((T*)(&v));
      //   }
      // end:
      //   return def;
      // }

     private:
      Document doc;
    };
  };
};  // namespace common

#endif  // __COMMON_CONFIG_HPP_
