
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

    optional<json> openConfig(){

        if(configData) {   // already content loaded
            return configData;
        }

        const auto PathToIROHA_HOME = [](){
            const auto p = getenv("IROHA_HOME");
            return p == nullptr ? "" : std::string(p);
        }();

        if (PathToIROHA_HOME.empty()) {
            logger::error("peer with json", "You must set IROHA_HOME!");
            exit(EXIT_FAILURE);
//            return nullopt;
        }

        const auto PathToSumeragiJson = PathToIROHA_HOME + "/config/sumeragi.json";
        std::ifstream ifs(PathToSumeragiJson);
        if(ifs.fail()) {
            logger::error("peer with json", "Not found: " + PathToSumeragiJson);
            exit(EXIT_FAILURE);
//            return nullopt;
        }

        std::istreambuf_iterator<char> it(ifs);
        std::string jsonStr(it, std::istreambuf_iterator<char>());

        logger::info("peer with json", "load json is " + jsonStr);

        try {
            configData = json::parse(jsonStr);
            return configData;
        } catch(...) {
            logger::error("peer with json", "Bad json!!");
            exit(EXIT_FAILURE);
//            return nullopt;
        }

    }

    std::string getMyPublicKey() {
        if(auto config = openConfig()) {
            return (*config)["me"]["publicKey"].get<std::string>();
        }
        return "";
    }

    std::string getPrivateKey() {
        if(auto config = openConfig()) {
            return (*config)["me"]["privateKey"].get<std::string>();
        }
        return "";
    }

    std::string getMyIp() {
        if(auto config = openConfig()) {
            return (*config)["me"]["ip"].get<std::string>();
        }
        return "";
    }

    std::vector<std::unique_ptr<peer::Node>> getPeerList() {
        std::vector<std::unique_ptr<peer::Node>> nodes;
        if(auto config = openConfig()) {
            for(const auto& peer : (*config)["group"].get<std::vector<json>>()){
                nodes.push_back(std::make_unique<peer::Node>(
                    peer["ip"].get<std::string>(),
                    peer["publicKey"].get<std::string>(),
                    1
                ));
            }
        }
        return nodes;
    }
};
