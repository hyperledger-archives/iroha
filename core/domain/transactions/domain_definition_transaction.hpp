#ifndef CORE_DOMAIN_TRANSACTIONS_DOMAINDEFINITIONTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_DOMAINDEFINITIONTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace domain_definition_transaction {
  MSGPACK_DEFINE(hash, type, domainName, accountPublicKey, signature, timestamp);
};

#endif  // CORE_DOMAIN_TRANSACTIONS_DOMAINDEFINITIONTRANSACTION_HPP_