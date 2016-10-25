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
        return "j0B8v4M/K853QpyHfgZo1towzNvNt4pQOyqDt0ewGy8=";// TODO: WIP
    }

    std::string getPrivateKey() {
        return "IDeSLBBYxhY9s2J4MgnKobLH9hGvaxR97B3g3yJ6NXrO5EYtyvlSw7s2VYFQLOzT31FK+0QtsKcKe1UWFxtXYA==";// TODO: WIP
    }

    std::vector<Node> getPeerList() {  
        std::vector<Node> res;
        // WIP use json loadr
        return res;
    }
};
