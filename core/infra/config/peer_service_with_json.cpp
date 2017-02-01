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
#include "../../crypto/base64.hpp"
#include "../../util/logger.hpp"

//#define DEBUG printf("%s(%d):", __func__, __LINE__);

namespace config {

    namespace {
        //  Which is better?
        //  1. (*config)["me"]["publicKey"] : easily comprehensible accessing json value
        //  2. (*config)[Me][PublicKey]     : can avoid typo
        const std::string PublicKey     = "publicKey";
        const std::string PrivateKey    = "privateKey";
        const std::string IP            = "ip";
        const std::string Name          = "name";
        const std::string Group         = "group";
        const std::string Me            = "me";
    }

    PeerServiceConfig::PeerServiceConfig() {}

    PeerServiceConfig& PeerServiceConfig::getInstance() {
        static PeerServiceConfig serviceConfig;
        return serviceConfig;
    }

    std::string PeerServiceConfig::getMyPublicKey() {
        if (auto config = openConfig(getConfigName())) {
            return (*config)[Me][PublicKey].get<std::string>();
        }
        return "";
    }

    std::string PeerServiceConfig::getPrivateKey() {
        if (auto config = openConfig(getConfigName())) {
            return (*config)[Me][PrivateKey].get<std::string>();
        }
        return "";
    }

    std::string PeerServiceConfig::getMyIp() {
        if (auto config = openConfig(getConfigName())) {
            return (*config)[Me][IP].get<std::string>();
        }
        return "";
    }

    namespace detail { namespace formatChecking {

        using NestedKeys = std::vector<std::string>;

        template<class T = std::string>
        bool ensureKeyValueFormat(json& partConfig, const NestedKeys& keys, const std::size_t keyIndex);

        template<class T>
        bool ensureValue(const std::string& key, T& value) {
            logger::fatal("peer service with json") << "Not implemented error";
            exit(EXIT_FAILURE);
        }

        template<>
        bool ensureValue(const std::string& key, std::string& value) {

            if (key == PublicKey) {
                // check base64
                return true;
            } else if (key == PrivateKey) {
                // check base64
                return true;
            } else if (key == IP) {
                // Duplicate (config_test.cpp)
                std::regex ipRegex("^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");//\"((([0-1]?\\d\\d?)|((2[0-4]\\d)|(25[0-5]))).){3}(([0-1]?\\d\\d?)|((2[0-4]\\d)|(25[0-5])))\"");
                std::smatch match;
                if (not std::regex_match(value, match, ipRegex)) {
                    logger::warning("peer service with json")
                        << "IP " << value << " looks like not a valid ip.";
                    return false;
                }
                return true;
            } else if (key == Name) {
                // Check invalid name
                return true;
            } else {
                logger::warning("peer service with json")
                    << "No match value: " << value;
                return false;
            }
        }

        template<>
        bool ensureValue(const std::string& key, std::vector<json>& value) {
            if (key == Group) {
                bool res = true;
                for (auto& e: value) {
                    // temporaly, follow getPeerList()
                    res = res && ensureKeyValueFormat(e, {IP},        0);
                    res = res && ensureKeyValueFormat(e, {Name},      0);
                    res = res && ensureKeyValueFormat(e, {PublicKey}, 0);
                }
                return res;
            }

            return false;
        }

        template <class T>
        bool ensureKeyValueFormat(json& partConfig, const NestedKeys& keys, const std::size_t keyIndex) {
            if (keyIndex == keys.size()) {
                return true;
            }

            json nextPartConfig;

            auto showWarning = [&keyIndex, &keys](const std::string& reason) {
                std::string parsedKeys;
                for (std::size_t i = 0; i < keyIndex; i++) {
                    if (i) parsedKeys += " : ";
                    parsedKeys += "\"" + keys[i] + "\"";
                }

                // currently cannot specify IP in group
                std::string where;
                if (not parsedKeys.empty()) {
                    where = " in " + parsedKeys;
                } else {
                    where = " in " + std::string("(maybe \"group\")");
                }

                logger::warning("peer service with json")
                    << reason << ": \"" + keys[keyIndex] + "\""
                    << where;
            };

            if (keyIndex + 1 == keys.size()) {
                try {
                    auto value = partConfig[keys[keyIndex]].get<T>();
                    return ensureValue<T>(keys[keyIndex], value);
                } catch(...) {
                    showWarning("Type mismatch");
                    return false;
                }
            } else {
                try {
                    nextPartConfig = partConfig[keys[keyIndex]];
                } catch(...) {
                    showWarning("Not found");
                    return false;
                }
            }

            return ensureKeyValueFormat<T>(nextPartConfig, keys, keyIndex + 1);
        }

        static bool ensureAllKeyValueFormats(json& config) {
            bool res = true;
            res = res && ensureKeyValueFormat <std::string>       (config, {Me, IP},          0);
            res = res && ensureKeyValueFormat <std::string>       (config, {Me, Name},        0);
            res = res && ensureKeyValueFormat <std::string>       (config, {Me, PublicKey},   0);
            res = res && ensureKeyValueFormat <std::string>       (config, {Me, PrivateKey},  0);
            res = res && ensureKeyValueFormat <std::vector<json>> (config, {Group},           0);
            return res;
        }

    }}

    bool PeerServiceConfig::ensureConfigFormat(const std::string& jsonStr) {
        bool res = true;
        try {
            auto config = json::parse(jsonStr);
            res = res && detail::formatChecking::ensureAllKeyValueFormats(config);
        } catch(...) {
            logger::error("peer service with json") << "Bad json.";
            exit(EXIT_FAILURE);
        }

        return res;
    }

    std::vector<std::unique_ptr<peer::Node>> PeerServiceConfig::getPeerList() {
        std::vector<std::unique_ptr<peer::Node>> nodes;
        if (auto config = openConfig(getConfigName())) {
            for (const auto& peer : (*config)[Group].get<std::vector<json>>()){
                nodes.push_back(std::make_unique<peer::Node>(
                    peer[IP].get<std::string>(),
                    peer[PublicKey].get<std::string>(),
                    1
                ));
            }
        }
        return nodes;
    }

    std::string PeerServiceConfig::getConfigName() {
        return "config/sumeragi.json";
    }
};
