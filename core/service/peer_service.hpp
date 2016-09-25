#ifndef __CORE_URL_SERVICE_HPP__
#define __CORE_URL_SERVICE_HPP__

#include <vector>
#include <string>

#include "../util/yaml_loader.hpp"

namespace service {

    namespace peer {

        class 

        std::vector<Node> getPeerList() {
            std::unique_ptr<yaml::YamlLoader> yamlLoader(new yaml::YamlLoader(std::string(getenv("IROHA_HOME")) + "/config/config.yml"));
            return std::move(yamlLoader->get<std::vector<std::string> >("peer", "ip"));
        }
    }
}

#endif