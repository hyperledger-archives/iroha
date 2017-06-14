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
#include "self_state.hpp"


#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <crypto/base64.hpp>
#include <crypto/signature.hpp>

namespace peer_service {
    enum State { PREPARE, READY, ACTIVE };
    namespace self_state {

        std::string _ip;
        std::string _public_key;
        std::string _private_key;

        State _state;

        void initializeMyKey() {
          if (_public_key.empty() || _private_key.empty()) {
            signature::KeyPair keyPair = signature::generateKeyPair();
            _public_key = base64::encode(keyPair.publicKey);
            _private_key = base64::encode(keyPair.privateKey);
          }
        }

        void initializeMyIp() {
          if (_ip.empty()) {
            int sockfd;
            struct ifreq ifr;

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
            ioctl(sockfd, SIOCGIFADDR, &ifr);
            close(sockfd);
            _ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
          }
        }


        std::string getPublicKey() {
          initializeMyKey();
          return _public_key;
        }
        std::string getPrivateKey() {
          initializeMyKey();
          return _private_key;
        }
        std::string getIp() {
          initializeMyIp();
          return _ip;
        }

        bool isLeader() {
        }

        State state() { return _state; }

        void activate() { _state = ACTIVE; }
        void stop() { _state = PREPARE; }
    };
};
