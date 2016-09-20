#ifndef CORE_DOMAIN_DOMAINRENEWALTRANSACTION_HPP_
#define CORE_DOMAIN_DOMAINRENEWALTRANSACTION_HPP_

#include <msgpack.hpp>

namespace DomainRenewalTransaction {
  MSGPACK_DEFINE(hash, type, domainToRenew, accountPublicKey);
};

#endif  // CORE_DOMAIN_DOMAINRENEWALTRANSACTION_HPP_
