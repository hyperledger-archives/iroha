#ifndef CORE_DOMAIN_TRANSACTIONS_SIGNATORYDELETETRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_SIGNATORYDELETETRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace SignatoryDeleteTransaction {
    MSGPACK_DEFINE(hash, type, accountPublicKey, signerToDeletePublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_SIGNATORYDELETETRANSACTION_HPP_
