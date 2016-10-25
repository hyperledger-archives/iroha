#include "../../core/consensus/sumeragi.hpp"
#include "../../core/repository/consensus/event_repository.hpp"
#include "../../core/model/transactions/transfer_transaction.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>

/*
 Sample Public key
 UKsNazVWDZ1oqNBoFpckirYDvwI/LyL9BqNYXVWCqmw=
 */

/*
 Sample Friend Key
 public: fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=
*/
int main(){
    std::string cmd;
    std::vector<std::unique_ptr<peer::Node>> nodes;
    nodes.push_back(std::make_unique<peer::Node>(
        "45.32.158.71",
        "fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=",
        1.0
    ));
    std::string pubKey = "UKsNazVWDZ1oqNBoFpckirYDvwI/LyL9BqNYXVWCqmw=";
    sumeragi::initializeSumeragi( pubKey, std::move(nodes));
    std::thread http_th( []() {
        sumeragi::loop();
    });

    while(1){
        std::cout << "in >> ";
        std::cin>> cmd;
        if(cmd == "quit") break;
        repository::event::add("80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b", std::make_unique<consensus_event::ConsensusEvent>(
            std::make_unique<transaction::TransferTransaction>(
                "fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=",
                pubKey,
                "domain",
                cmd
            )
        ));

    }

    http_th.detach();

    return 0;
}
