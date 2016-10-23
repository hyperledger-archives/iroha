#include <vector>
#include <string>
#include <memory>

#include "peer_service.hpp"

namespace peer{

    std::string Node::getIP() const{
        return ip;
    }

    std::string Node::getPublicKey() const{
        return publicKey;
    }

    double Node::getTrustScore() const {
        return 0; // TODO: WIP
    }

    std::string getMyPublicKey() {
        return "Base64";// TODO: WIP
    }

    std::string getPrivateKey() {
        return "Base64";// TODO: WIP
    }

    std::vector<Node> getPeerList() {  
        std::vector<Node> res;
        // WIP use json loadr
        return res;
    }
};
