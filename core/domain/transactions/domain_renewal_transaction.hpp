#ifndef CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace DomainRenewalTransaction {
  MSGPACK_DEFINE(hash, type, domainToRenew, accountPublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_DOMAINRENEWALTRANSACTION_HPP_
