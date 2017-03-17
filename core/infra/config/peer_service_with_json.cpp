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

#include <deque>
#include <regex>
#include <algorithm>
#include <crypto/base64.hpp>
#include <util/logger.hpp>
#include <util/exception.hpp>
#include <consensus/connection/connection.hpp>
#include <json.hpp>
#include <infra/protobuf/api.pb.h>
#include <transaction_builder/transaction_builder.hpp>
#include <repository/transaction_repository.hpp>
#include "peer_service_with_json.hpp"
#include "config_format.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;

PeerServiceConfig::PeerServiceConfig() {
}

PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}

// TODO: I could not use getParam method because of sumeragi.json is hierarchical.
// TODO: Which config file these parameter should be set? It's not sumeragi but hijiri.

std::string PeerServiceConfig::getMyPublicKeyWithDefault(const std::string& defaultValue) {
  auto config = this->getConfigData();
  if (!config.is_null()) {
    return this->getConfigData()["me"].value("publicKey", defaultValue);
  }
  return defaultValue;
}

std::string PeerServiceConfig::getMyPrivateKeyWithDefault(const std::string& defaultValue) {
  auto config = this->getConfigData();
  if (!config.is_null()) {
    return this->getConfigData()["me"].value("privateKey", defaultValue);
  }
  return defaultValue;
}

std::string PeerServiceConfig::getMyIpWithDefault(const std::string& defaultValue) {
  auto config = this->getConfigData();
  if (!config.is_null()) {
    return this->getConfigData()["me"].value("ip", defaultValue);
  }
  return defaultValue;
}

double PeerServiceConfig::getMaxTrustScoreWithDefault(double defaultValue) {
  return this->getConfigData().value("max_trust_score", defaultValue);
}

void PeerServiceConfig::parseConfigDataFromString(std::string&& jsonStr) {
  try {
    if (!ConfigFormat::getInstance().ensureFormatSumeragi(jsonStr)) {
      throw exception::ParseFromStringException(getConfigName());
    }
    _configData = json::parse(std::move(jsonStr));
  } catch (exception::ParseFromStringException& e) {
    logger::warning("peer service config") << e.what();
    logger::warning("peer service config") << getConfigName() << " is set to be default.";
  }
}

std::string PeerServiceConfig::getConfigName() {
  return "config/sumeragi.json";
}

double PeerServiceConfig::getMaxTrustScore() {
  return this->getMaxTrustScoreWithDefault(10.0); // WIP to support trustRate = 10.0
}

// TODO: this is temporary solution
std::vector<json> PeerServiceConfig::getGroup() {
  auto config = this->getConfigData();
  if (!config.is_null()) {
     return getConfigData()["group"].get<std::vector<json>>();
  }
  return std::vector<json>({
      json({
        {"ip","172.17.0.3"},
        {"name","mizuki"},
        {"publicKey","jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0="}
      }),
      json({
        {"ip","172.17.0.4"},
        {"name","natori"},
        {"publicKey","Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY="}
      }),
      json({
        {"ip","172.17.0.5"},
        {"name","kabohara"},
        {"publicKey","f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig="}
      }),
      json({
        {"ip","172.17.0.6"},
        {"name","samari"},
        {"publicKey","Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU="}
      })
    });
}
