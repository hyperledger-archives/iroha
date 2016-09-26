#include <vector>
#include <string>
#include <memory>

#include "../util/yaml_loader.hpp"
#include "peer_service.hpp"

namespace peer{

    std::string Node::getIP() const{
        return ip;
    }

    std::string Node::getPublicKey() const{
        return publicKey;
    }


    std::string getMyPublicKey() {
        return "Base64";// WIP
    }

    std::string getPrivateKey() {
        return "Base64";// WIP
    }

    std::vector<Node> getPeerList() {
        std::unique_ptr<yaml::YamlLoader> yamlLoader(new yaml::YamlLoader(std::string(getenv("IROHA_HOME")) + "/config/config.yml"));
        return std::vector<Node>();
        //return std::move(yamlLoader->get<std::vector<std::string> >("peer", "ip"));
    }
};
