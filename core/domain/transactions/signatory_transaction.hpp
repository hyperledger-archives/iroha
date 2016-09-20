#ifndef CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace signatory_transaction {
    MSGPACK_DEFINE(prevTxHash, hash, type, transactionToSign, accountPublicKey, signerPublicKey);
};  // namespace signatory_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_SIGNATORYTRANSACTION_HPP_
