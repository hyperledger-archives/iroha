#ifndef CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_
#define CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_

#include <msgpack.hpp>

namespace SignatoryTransaction {
    MSGPACK_DEFINE(prevTxHash, hash, type, transactionToSign, accountPublicKey, signerPublicKey);
};

#endif  // CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_
