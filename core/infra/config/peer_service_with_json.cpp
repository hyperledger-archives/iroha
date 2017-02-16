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

#include <deque>
#include <regex>
#include <crypto/base64.hpp>
#include <util/logger.hpp>
#include <util/exception.hpp>
#include <util/use_optional.hpp>
#include <json.hpp>
#include "peer_service_with_json.hpp"
#include "config_format.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;


PeerServiceConfig::PeerServiceConfig() {}

PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}

std::string PeerServiceConfig::getMyPublicKey() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["publicKey"].get<std::string>();
  }
  return "";
}

std::string PeerServiceConfig::getMyPrivateKey() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["privateKey"].get<std::string>();
  }
  return "";
}

std::string PeerServiceConfig::getMyIp() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["ip"].get<std::string>();
  }
  return "";
}

std::vector<std::unique_ptr<peer::Node>> PeerServiceConfig::getPeerList() {
  std::vector<std::unique_ptr<peer::Node>> nodes;
  if (auto config = openConfig(getConfigName())) {
    for (const auto& peer : (*config)["group"].get<std::vector<json>>()) {
      nodes.push_back(std::make_unique<peer::Node>(
          peer["ip"].get<std::string>(), peer["publicKey"].get<std::string>(),
          1));
    }
  }
  return nodes;
}


void PeerServiceConfig::parseConfigDataFromString(std::string&& jsonStr) {
  try {
    if (not ConfigFormat::getInstance().ensureFormatSumeragi(jsonStr)) {
      throw exception::ParseFromStringException("sumeragi");
    }
    _configData = json::parse(std::move(jsonStr));
  } catch (exception::ParseFromStringException& e) {    
    logger::warning("peer service config") << e.what();
    logger::warning("peer service config") << getConfigName() << " is set to be default.";

    // default sumeragi.json
    _configData = json::parse(R"({
      "me":{
        "ip":"172.17.0.6",
        "name":"samari",
        "publicKey":"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=",
        "privateKey":"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA=="
      },
      "group":[
        {
          "ip":"172.17.0.3",
          "name":"mizuki",
          "publicKey":"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0="
        },
        {
          "ip":"172.17.0.4",
          "name":"natori",
          "publicKey":"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY="
        },
        {
          "ip":"172.17.0.5",
          "name":"kabohara",
          "publicKey":"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig="
        },
        {
          "ip":"172.17.0.6",
          "name":"samari",
          "publicKey":"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU="
        }
      ]
    })");
  }
}

std::string PeerServiceConfig::getConfigName() {
  return "config/sumeragi.json";
}
