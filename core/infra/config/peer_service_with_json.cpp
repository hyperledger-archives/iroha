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
#include "peer_service_with_json.hpp"
#include <crypto/base64.hpp>
#include <util/logger.hpp>
#include <util/use_optional.hpp>
#include <util/exception.hpp>
#include <json.hpp>

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

std::string PeerServiceConfig::getConfigName() {
  return "config/sumeragi.json";
}

namespace detail { namespace formatChecking {

bool ensureFormat(json& actualConfig, json& formatConfig, const std::string& history) {

  if (actualConfig.type() != formatConfig.type()) {
    logger::warning("peer service with json")
      << "type must be " << formatConfig.type() << ", but is " << actualConfig.type();
    return false;
  } else {

    if (actualConfig.is_object()) {

      for (auto it = formatConfig.begin(); it != formatConfig.end(); it++) {
        if (actualConfig.find(it.key()) == actualConfig.end()) {
          logger::warning("peer service with json")
            << "Not found: \"" << it.key() << "\" in " << history;
          return false;
        }
      }

      std::vector<json::iterator> configIters;

      bool res = true;

      for (auto it = actualConfig.begin(); it != actualConfig.end(); it++) {
        if (formatConfig.find(it.key()) == formatConfig.end()) {
          logger::warning("peer service with json")
            << "Unused keys: \"" << it.key() << "\" in " << history;
          return false;
        } else {
          res = res && ensureFormat(
            it.value(),
            formatConfig.find(it.key()).value(),
            history + " : \"" + it.key() + "\""
            );
        }
      }

      return res;
    }
    else if (actualConfig.is_string()) {

      const auto& format = formatConfig.get<std::string>();
      const auto& value  = actualConfig.get<std::string>();

      if (format == "ip") {
        // cannot use config file because nlohmann::json cannot parse string
        // std::regex ipRegex(FormatRegExConfig::getInstance().getIPRegEx());
        std::regex ipRegex("^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
        if (not std::regex_match(value, ipRegex)) {
          logger::warning("peer service with json")
            << "IP " << value << " looks like not a valid ip.";
          return false;
        }
        return true;
      }
      else if (format == "publicKey") {
        // currently do nothing
        return true;
      }
      else if (format == "privateKey") {
        // currently do nothing
        return true;
      }
      else if (format == "*") {
        // DO NOTHING. Any string is accepted.
        return true;
      }
      else {
        throw exception::NotImplementedException("ensureFormat", "peer service with json");
      }
    }
    else if (actualConfig.is_array()) {
      auto formats = formatConfig.get<std::vector<json>>();
      auto values  = actualConfig.get<std::vector<json>>();

      if (formats.size() != 1) {
        throw std::domain_error("array size should be 1 , but is " + std::to_string(formats.size()) + ")");
      }

      auto& format = formats.front();

      bool res = true;
      for (auto& value: values) {
        res = res && ensureFormat(value, format, history);
      }
      return res;
    }
    else {
      throw std::invalid_argument("not implemented error");
    }
  }
}

}}

bool PeerServiceConfig::ensureConfigFormat(const std::string& configStr) {

  using detail::formatChecking::ensureFormat;

  json config, formatConfig;

  try {
    config = json::parse(configStr);
  } catch(std::exception& e) {
    return false;
  }

  try {
    formatConfig = json::parse(R"(
        {
          "me":{
            "ip":"ip",
            "name":"*",
            "publicKey":"publicKey",
            "privateKey":"privateKey"
          },
          "group":[
            {
              "ip":"ip",
              "name":"*",
              "publicKey":"publicKey"
            }
          ]
        }
      )");
  } catch(std::exception& e) {
    throw std::domain_error("cannot parse config format. " + std::string(e.what()));
  }

  return ensureFormat(config, formatConfig, "(root)");
}