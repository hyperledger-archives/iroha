#ifndef CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace add_peer_transaction {
  MSGPACK_DEFINE(hash, type, domainToDefine, accountPublicKey);
};  // namespace add_peer_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_ADDPEERTRANSACTION_HPP_