#include "../../core/consensus/sumeragi.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>


/*
 Sample Public key
 UKsNazVWDZ1oqNBoFpckirYDvwI/LyL9BqNYXVWCqmw=
 */

/*
 Sample Friend Key
 public: fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=
*/
int main(){

    std::vector<std::unique_ptr<peer::Node>> nodes;
    nodes.push_back(std::make_unique<peer::Node>(
        "45.32.158.71",
        "fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=",
        1.0
    ));
    std::string pubKey = "UKsNazVWDZ1oqNBoFpckirYDvwI/LyL9BqNYXVWCqmw=";
    sumeragi::initializeSumeragi( pubKey, std::move(nodes));
    sumeragi::loop();

    return 0;
}