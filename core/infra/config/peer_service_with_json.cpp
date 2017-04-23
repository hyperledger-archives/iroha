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
#include <utils/exception.hpp>
#include <utils/logger.hpp>

#include <infra/config/config_format.hpp>
#include <infra/config/peer_service_with_json.hpp>

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;

PeerServiceConfig::PeerServiceConfig() {}

PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}

std::string PeerServiceConfig::getMyPublicKey() {
  return getParamWithAssert<std::string>({"me", "publicKey"});
}


std::string PeerServiceConfig::getMyPrivateKey() {
  return getParamWithAssert<std::string>({"me","privateKey"});
}

std::string PeerServiceConfig::getMyIp() {
  return getParamWithAssert<std::string>({"me","ip"});
}

double PeerServiceConfig::getMaxTrustScore(double defaultValue) {
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

std::vector<json> PeerServiceConfig::getGroup() {
  return getParamWithAssert<std::vector<json>>({"group"}); // WIP ASSERT FALSE;
}
