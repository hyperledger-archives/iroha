
#include "../../service/peer_service.hpp"
#include "../../util/logger.hpp"
#include "../../util/use_optional.hpp"

#include <iostream>  // for debug writing
#include <cstdlib>

#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <json.hpp>
#include <vector>

namespace peer {

    using json = nlohmann::json;
    static optional<json> configData;

    namespace detail {
        std::string openJSONText(const std::string& PathToJSONFile) {
            std::ifstream ifs(PathToJSONFile);
            if (ifs.fail()) {
                logger::error("peer with json") << "Not found: " << PathToJSONFile;
                exit(EXIT_FAILURE);
            }

            std::istreambuf_iterator<char> it(ifs);
            return std::string(it, std::istreambuf_iterator<char>());
        }

        void setConfigData(std::string&& jsonStr) {
            try {
                configData = json::parse(std::move(jsonStr));
            } catch(...) {
                logger::error("peer with json") << "Bad json!!";
                exit(EXIT_FAILURE);
            }
        }
        
        json openConfig() {

            if (configData) {   // already content loaded
                return *configData;
            }

            const auto PathToIROHA_HOME = [](){
                const auto p = getenv("IROHA_HOME");
                return p == nullptr ? "" : std::string(p);
            }();

            if (PathToIROHA_HOME.empty()) {
                logger::error("peer with json") << "You must set IROHA_HOME!";
                exit(EXIT_FAILURE);
            }

            auto jsonStr = openJSONText(PathToIROHA_HOME + "/config/sumeragi.json");
            
            logger::info("peer with json") << "load json is " << jsonStr;

            setConfigData(std::move(jsonStr));

            return *configData;
        }
    }

    std::string getMyPublicKey() {
        try {
            return detail::openConfig()["me"]["publicKey"].get<std::string>();
        } catch(...) {
            return "";
        }
    }

    std::string getPrivateKey() {
        try {
            return detail::openConfig()["me"]["privateKey"].get<std::string>();
        } catch(...) {
            return "";
        }
    }

    std::string getMyIp() {
        try {
            return detail::openConfig()["me"]["ip"].get<std::string>();
        } catch(...) {
            return "";
        }
    }

    std::vector<std::unique_ptr<peer::Node>> getPeerList() {
        try {
            std::vector<std::unique_ptr<peer::Node>> nodes;
            for (const auto& peer : detail::openConfig()["group"].get<std::vector<json>>()){
                nodes.push_back(std::make_unique<peer::Node>(
                    peer["ip"].get<std::string>(),
                    peer["publicKey"].get<std::string>(),
                    1
                ));
            }
            return nodes;
        } catch(...) {
            return {};
        }
    }
};