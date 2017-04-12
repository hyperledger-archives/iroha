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


PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}
// ToDo We can make more sort it. ===
std::string PeerServiceConfig::getMyPublicKeyWithDefault(
    const std::string& defaultValue) {
  if (auto config = getConfigData()) {
    return config["me"]["publicKey"].get<std::string>();
  }
  return defaultValue;
}
std::string PeerServiceConfig::getMyPrivateKeyWithDefault(
    const std::string& defaultValue) {
  if (auto config = getConfigData()) {
    return config["me"]["privateKey"].get<std::string>();
  }
  return defaultValue;
}
std::string PeerServiceConfig::getMyIpWithDefault(
    const std::string& defaultValue) {
  if (auto config = getConfigData()) {
    return config["me"]["ip"].get<std::string>();
  }
  return defaultValue;
}
bool PeerServiceConfig::isExistIP(const std::string& ip) {
  // ToDo
  return false;
  // return findPeerIP( std::move(ip) ) != peerList.end();
}
double PeerServiceConfig::getMaxTrustScoreWithDefault(double defaultValue) {
  return getConfigData().value("max_trust_score", defaultValue);
}
size_t PeerServiceConfig::getMaxFaultyScoreWithDefault(size_t defaultValue) {
  // ToDo
  return 1;  // getConfigData().value("max_trust_score", defaultValue);
}

std::vector<json> PeerServiceConfig::getGroup() {
  auto config = getConfigData();
  if (!config.is_null()) {
    return getConfigData()["group"].get<std::vector<json>>();
  }

  // default value
  return std::vector<json>(
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
}
double PeerServiceConfig::getMaxTrustScore() {
  return this->getMaxTrustScoreWithDefault(
      10.0);  // WIP to support trustRate = 10.0
}