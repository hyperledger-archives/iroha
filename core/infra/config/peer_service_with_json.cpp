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
#include <utils/expected.hpp>
#include <utils/logger.hpp>

#include "config_format.hpp"
#include "peer_service_with_json.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;

PeerServiceConfig::PeerServiceConfig() noexcept {}

PeerServiceConfig& PeerServiceConfig::getInstance() noexcept {
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
bool PeerServiceConfig::isExistIP(const std::string& ip) {
  // ToDo
  return true;
  // return findPeerIP( std::move(ip) ) != peerList.end();
}
double PeerServiceConfig::getMaxTrustScoreWithDefault(double defaultValue) {
  return getParam<double>({"max_trust_score"}, defaultValue);
}

VoidHandler PeerServiceConfig::parseConfigDataFromString(
    std::string&& jsonStr) {
  auto res = ConfigFormat::getInstance().ensureFormatSumeragi(jsonStr);

  if (res) {
    _configData = json::parse(std::move(jsonStr));
    return {};
  } else {
    return makeUnexpected(exception::ConfigException(
        "PeerService",
        "Failed parse config. " + getConfigName() + " is set to be default."));
  }
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

double PeerServiceConfig::getMaxTrustScore() {
  return this->getMaxTrustScoreWithDefault(
      10.0);  // WIP to support trustRate = 10.0
}