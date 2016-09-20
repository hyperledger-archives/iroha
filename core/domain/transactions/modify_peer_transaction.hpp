#ifndef CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace modify_peer_transaction {
  MSGPACK_DEFINE(hash, type, domainToDefine, accountPublicKey);
};  // namespace modify_peer_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_MODIFYPEERTRANSACTION_HPP_