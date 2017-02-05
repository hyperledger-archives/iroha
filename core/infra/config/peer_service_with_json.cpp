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

bool ensureFormat(json& config, json& basicConfig, const std::string& history) {

  if (config.type() != basicConfig.type()) {
    logger::warning("peer service with json")
      << "Type mismatch: actual: " << config << "\nexpected: " << basicConfig;
    return false;
  } else {

    if (config.is_object()) {

      for (auto it = basicConfig.begin(); it != basicConfig.end(); it++) {
        if (config.find(it.key()) == config.end()) {
          logger::warning("peer service with json")
            << "Not found: \"" << it.key() << "\" in " << history;
          return false;
        }
      }

      std::vector<json::iterator> configIters;

      bool res = true;

      for (auto it = config.begin(); it != config.end(); it++) {
        if (basicConfig.find(it.key()) == basicConfig.end()) {
          logger::warning("peer service with json")
            << "Unused keys: \"" << it.key() << "\" in " << history;
          return false;
        } else {
          res = res && ensureFormat(
            it.value(),
            basicConfig.find(it.key()).value(),
            history + " : \"" + it.key() + "\""
            );
        }
      }

      return res;
    }
    else if (config.is_string()) {

      const auto& format = basicConfig.get<std::string>();
      const auto& value  = config.get<std::string>();

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
        logger::fatal("peer service with json")
          << "Not implemented format (format JSON file error)\n"
          << "Unknown format: \"" << format << "\"";
        exit(EXIT_FAILURE);
      }
    }
    else if (config.is_array()) {
      auto formats = basicConfig.get<std::vector<json>>();
      auto values  = config.get<std::vector<json>>();

      if (formats.size() != 1) {
        logger::fatal("peer service with json")
          << "array size should be 1 (given " << std::to_string(formats.size())
          << "). (format JSON file error)";
        exit(EXIT_FAILURE);
      }

      auto& format = formats.front();

      bool res = true;
      for (auto& value: values) {
        res = res && ensureFormat(value, format, history);
      }
      return res;
    }
    else {
      logger::fatal("peer service with json")
        << "Not implemented type (format JSON file error)";
      exit(EXIT_FAILURE);
    }

    // NO ARRIVAL.
    assert(false);
  }
}

}}

bool PeerServiceConfig::ensureConfigFormat(const std::string& jsonStr) {
  using detail::formatChecking::ensureFormat;

  auto try_parse = [](const std::string& str) -> optional<json> {
    try {
      return make_optional<json>(json::parse(str));
    } catch(...) {
      logger::warning("peer service with json") << "Bad json.";
      return nullopt;
    }
  };

  if (auto config = try_parse(jsonStr)) {
    const std::string formatJsonStr = "{\"me\":{\"ip\":\"ip\",\"name\":\"*\",\"publicKey\":\"publicKey\",\"privateKey\":\"privateKey\"},\"group\":[{\"ip\":\"ip\",\"name\":\"*\",\"publicKey\":\"publicKey\"}]}";
    if (auto basicConfig = try_parse(formatJsonStr)) {
      return ensureFormat(*config, *basicConfig, "(root)");
    }
  }
  return false;
}