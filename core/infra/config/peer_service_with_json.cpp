/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language gover
ning permissions and
limitations under the License.
*/

#include <json.hpp>
#include <util/exception.hpp>
#include <util/logger.hpp>

#include "config_format.hpp"
#include "peer_service_with_json.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;

PeerServiceConfig::PeerServiceConfig() {}

PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}

std::string PeerServiceConfig::getMyPublicKeyWithDefault(
    const std::string& defaultValue) {
  return getParam<std::string>({"me", "publicKey"}, defaultValue);
}

std::string PeerServiceConfig::getMyPrivateKeyWithDefault(
    const std::string& defaultValue) {
  return getParam<std::string>({"me", "privateKey"}, defaultValue);
}

std::string PeerServiceConfig::getMyIpWithDefault(
    const std::string& defaultValue) {
  return getParam<std::string>({"me", "ip"}, defaultValue);
}

double PeerServiceConfig::getMaxTrustScoreWithDefault(double defaultValue) {
  return getParam<double>({"max_trust_score"}, defaultValue);
}

void PeerServiceConfig::parseConfigDataFromString(std::string&& jsonStr) {
  try {
    if (!ConfigFormat::getInstance().ensureFormatSumeragi(jsonStr)) {
      throw exception::ParseFromStringException(getConfigName());
    }
    _configData = json::parse(std::move(jsonStr));
  } catch (exception::ParseFromStringException& e) {
    logger::warning("peer service config") << e.what();
    logger::warning("peer service config") << getConfigName()
                                           << " is set to be default.";
  }
}

std::string PeerServiceConfig::getConfigName() {
  return "config/sumeragi.json";
}

double PeerServiceConfig::getMaxTrustScore() {
  return this->getMaxTrustScoreWithDefault(
      10.0);  // WIP to support trustRate = 10.0
}

std::vector<json> PeerServiceConfig::getGroup() {
  std::vector<json> defaultValue(
      {json({{"ip", "172.17.0.3"},
             {"name", "mizuki"},
             {"publicKey", "jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0="}}),
       json({{"ip", "172.17.0.4"},
             {"name", "natori"},
             {"publicKey", "Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY="}}),
       json({{"ip", "172.17.0.5"},
             {"name", "kabohara"},
             {"publicKey", "f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig="}}),
       json({{"ip", "172.17.0.6"},
             {"name", "samari"},
             {"publicKey", "Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU="}})});

  return getParam<std::vector<json>>({"group"}, defaultValue);
}
