#ifndef CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace AddPeerTransaction {
  MSGPACK_DEFINE(hash, type, domainToDefine, accountPublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_