
#include "../../service/peer_service.hpp"

#include <iostream>  // for debug writing
#include <string>    // useful for reading and writing

#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <json.hpp>
#include <vector>

namespace peer {

    using json = nlohmann::json;

    std::string openConfig(){
        std::ifstream ifs("../../../config/sumeragi.json");
        if(ifs.fail()) {
            std::cerr << "Fileが見つかりません" << std::endl;
            return "";
        }
        std::istreambuf_iterator<char> it(ifs);
        std::istreambuf_iterator<char> last;
        std::string res(it, last);
        return res;
    }

    std::string getMyPublicKey(){
        try{
            auto data = json::parse(openConfig());
            return data["me"]["publicKey"].get<std::string>();
        }catch(...){
            return "";
        }
    }

    std::string getPrivateKey(){
        try{
            auto data = json::parse(openConfig());
            return data["me"]["privateKey"].get<std::string>();
        }catch(...){
            return "";
        }
    }

    std::vector<Node> getPeerList(){
        std::vector<Node> nodes;
        try{
            auto data = json::parse(openConfig());
            for(const auto& peer : data["group"].get<std::vector<json>>()){
                nodes.push_back(Node(
                    peer["ip"].get<std::string>(),
                    peer["privateKey"].get<std::string>(),
                    1
                ));
            }
            return nodes;
        }catch(...){
            return nodes;
        }
    }
};