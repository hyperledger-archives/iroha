#ifndef CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace ModifyPeerTransaction {
  MSGPACK_DEFINE(hash, type, domainToDefine, accountPublicKey);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_