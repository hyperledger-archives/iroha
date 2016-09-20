#ifndef CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace SignatoryAddTransaction {
    MSGPACK_DEFINE(hash, type, accountPublicKey, signerToAddPublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_
