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

#ifndef IROHA_CONFIG_H
#define IROHA_CONFIG_H

#include <fstream>  // ifstream, ofstream
#include <util/logger.hpp>
#include <util/use_optional.hpp>
#include <json.hpp>

namespace config {

using json = nlohmann::json;

class AbstractConfigManager {
 protected:
  optional<json> openConfig(const std::string& configName) {
    if (_configData) {  // content is already loaded
      return _configData;
    }

    auto iroha_home = getenv("IROHA_HOME");
    if (iroha_home == nullptr) {
      logger::error("config") << "Set environment variable IROHA_HOME";
      exit(EXIT_FAILURE);
    }

    auto configFolderPath = std::string(iroha_home) + "/";
    auto jsonStr = readConfigData(configFolderPath + configName);

    logger::debug("config") << "load json is " << jsonStr;

    parseConfigDataFromString(std::move(jsonStr));

    return _configData;
  }

  std::string readConfigData(const std::string& pathToJSONFile) {
    std::ifstream ifs(pathToJSONFile);
    if (ifs.fail()) {
      logger::error("config") << "Not found: " << pathToJSONFile;
      return nullptr;
    }

    std::istreambuf_iterator<char> it(ifs);
    return std::string(it, std::istreambuf_iterator<char>());
  }

  virtual void parseConfigDataFromString(std::string&& jsonStr) {
    try {
      _configData = json::parse(std::move(jsonStr));
    } catch (...) {
      logger::error("config") << "Can't parse json: " << getConfigName();
    }
  }

 public:
  virtual std::string getConfigName() = 0;
  optional<json> getConfigData() { return this->_configData; }

 protected:
  optional<json> _configData;
};
}

#endif  // IROHA_CONFIG_H
