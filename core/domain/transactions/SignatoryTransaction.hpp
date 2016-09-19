#ifndef CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_

#include "AbstractTransaction.hpp
#include <msgpack.hpp>

namespace SignatoryTransaction {
    MSGPACK_DEFINE(prevTxHash, hash, type, transactionToSign, accountPublicKey, signerPublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_
