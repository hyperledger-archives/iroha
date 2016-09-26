#ifndef __CORE_URL_SERVICE_HPP__
#define __CORE_URL_SERVICE_HPP__

#include <vector>
#include <string>

#include "../util/yaml_loader.hpp"


namespace peer{
    class Node {
            std::string ip;
            std::string publicKey;
        public:

            Node(
                std::string aip,
                std::string apubkey
            ):
                ip(aip),
                publicKey(apubkey)
            {}
            
            /*
            virtual ~Node() = default; // make dtor virtual
            Node(Node&&) = default;  // support moving
            Node& operator = (Node&&) = default;
            Node(const Node&) = default; // support copying
            Node& operator = (const Node&) = default;
            
            virtual std::string getIP() = 0;
            virtual std::string getPublicKey() = 0;
            */

            std::string getIP() const{
                return ip;
            }

            std::string getPublicKey() const{
                return publicKey;
            }
    };


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

#endif
