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
#include <iostream>
#include <membership_service/peer_service.hpp>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Plz input target IP " << std::endl;
        std::cout << "Usage: check_ametsuchi publicKey ip-address ledger" << std::endl;
        return 1;
    }
    std::cout << "OwnPublicKey:" << argv[1] << std::endl;
    std::cout << "TargetIP:"     << argv[2] << std::endl;
    std::cout << "LedgerName:"   << argv[3] << std::endl;

    const auto peer = ::peer::Node( argv[2],  argv[1], argv[3]);
    ::peer::transaction::isssue::add(::peer::myself::getIp(), peer);

    std::cout << "== Done!! ==" << std::endl;
    return 0;
}