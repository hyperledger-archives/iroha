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
            );
            
            /*
            virtual ~Node() = default; // make dtor virtual
            Node(Node&&) = default;  // support moving
            Node& operator = (Node&&) = default;
            Node(const Node&) = default; // support copying
            Node& operator = (const Node&) = default;
            
            virtual std::string getIP() = 0;
            virtual std::string getPublicKey() = 0;
            */

            std::string getIP() const;
            std::string getPublicKey() const;
    };


    std::string getMyPublicKey();

    std::string getPrivateKey();

    std::vector<Node> getPeerList();
};

#endif
