#ifndef CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace signatory_add_transaction {
    MSGPACK_DEFINE(hash, type, accountPublicKey, signerToAddPublicKey, signature, timestamp);
};  // namespace signatory_add_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_SIGNATORYADDTRANSACTION_HPP_
