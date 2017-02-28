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

#ifndef PEER_SERVICE_WITH_JSON_HPP
#define PEER_SERVICE_WITH_JSON_HPP

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <service/peer_service.hpp>
#include "abstract_config_manager.hpp"

namespace config {

class PeerServiceConfig : config::AbstractConfigManager {
 private:
  static std::vector<peer::Node> peerList;
  PeerServiceConfig();
  void initialziePeerList_from_json();

  bool isExistIP( const std::string& );
  bool isExistPublicKey( const std::string& );
  std::vector<peer::Node>::iterator findPeerIP( const std::string& );
  std::vector<peer::Node>::iterator findPeerPublicKey( const std::string& );
  std::vector<peer::Node>::iterator findPeer( peer::Node& );

 protected:
  void parseConfigDataFromString(std::string&& jsonStr) override;

 public:
  static PeerServiceConfig &getInstance();

  std::string getMyPublicKey();
  std::string getMyPrivateKey();
  std::string getMyIp();

  std::vector<std::unique_ptr<peer::Node>> getPeerList();
  std::vector<std::string> getIpList();

  bool addPeer( peer::Node& );
  bool removePeer( peer::Node& );
  bool updatePeer( peer::Node& );
  bool isLeaderMyPeer();

  virtual std::string getConfigName();
};
}

#endif  // PEER_SERVICE_WITH_JSON_HPP
