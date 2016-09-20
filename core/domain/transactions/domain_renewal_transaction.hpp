#ifndef CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace domain_renewal_transaction {
  MSGPACK_DEFINE(hash, type, domainToRenew, accountPublicKey);
};  // namespace domain_renewal_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_
