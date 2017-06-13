//
// Created by TaiseiIgarashi on 2017/06/14.
//

#ifndef IROHA_RUNTIME_RUNTIME_HPP
#define IROHA_RUNTIME_RUNTIME_HPP

#include <block.pb.h>

namespace runtime {
    using Transaction = iroha::protocol::Transaction;
    void processTransaction(const Transaction&);
}

#endif //IROHA_RUNTIME_HPP
