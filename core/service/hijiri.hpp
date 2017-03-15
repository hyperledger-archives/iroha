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

#ifndef __CORE_HIJIRI_SERVICE_HPP__
#define __CORE_HIJIRI_SERVICE_HPP__


#include <vector>
#include <set>
#include <map>
#include <queue>
#include <service/peer_service.hpp>
#include "abstract_config_manager.hpp"

namespace hijiri {

    struct Context {
        std::vector<peer::Node> peerList;
        PeerEvent peer_event;
        bool is_active;
    };

    namespace detail {
        void initialziePeerList_from_json();
        std::vector<peer::Node>::iterator findPeerIP(const std::string &ip);
        std::vector<peer::Node>::iterator findPeerPublicKey(const std::string &publicKey);
    }

    namespace myself {
        std::string getPublicKey(); // getMyPublicKey();

        std::string getPrivateKey(); // getMyPrivateKey();

        std::string getIp(); // getMyIp();

        bool isActive(); // old: isMyActive
        void activate();

        void stop();

        // equatl to isSumeragi
        bool isLeader(); // old isLeaderMyPeer()
    }

    namespace config {
        double getMaxTrustScore();
    }

    namespace membership {
        size_t getMaxFaulty();

        std::vector<std::unique_ptr<peer::Node>> getPeerList();

        std::vector<std::string> getIpList();

        // is exist which peer?
        bool isExistIP(const std::string &);

        bool isExistPublicKey(const std::string &);

        std::unique_ptr<peer::Node> leaderPeer();
    }

    namespace amagimi {
        // 八苦を滅する尼公
        // it meaans that monitaring peers.

        // check are broken? peer
        void check( const std::string& ip); // void checkBrokenPeer(const std::string &ip);
        // [WIPn] does we need it? void checkAll();
    }

    // Initialize
    namespace izanami {
        void finidhed(); //void finishedInitializePeer();

        //invoke next to addPeer
        bool started();//bool sendAllTransaction( const peer::Node& );
    }

    namespace transaction {
        namespace isssue {
            // invoke to issue transaction
            void add( const peer::Node& );//void toIssue_addPeer( const peer::Node& );
            void distruct( const std::string& );//void toIssue_distructPeer( const std::string &publicKey );
            void remove( const std::string& );//void toIssue_removePeer( const std::string &publicKey );
            void credit( const std::string& );//void toIssue_creditPeer( const std::string &publicKey );
        }
        namespace executor {
            // invoke when execute transaction
            bool add( const peer::Node& );//bool addPeer( const peer::Node& );
            bool remove( const std::string& );//bool removePeer( const std::string &publicKey );
            bool update( const std::string&, const peer::Node& );//bool updatePeer( const std::string& publicKey, const peer::Node& peer );
        }
        namespace validator {
            // invoke when validator transaction
            bool add( const peer::Node& );//bool validate_addPeer( const peer::Node& );
            bool remove( const std::string& );//bool validate_removePeer( const std::string &publicKey );
            bool update( const std::string&, const peer::Node& );//bool validate_updatePeer( const std::string& publicKey, const peer::Node& peer );
        }
    }

}

#endif
