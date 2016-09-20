#ifndef CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_

#include "abstract_transaction.hpp"
#include <msgpack.hpp>

namespace transfer_transaction {
    AbstractTransaction::TransactionType type;
    unsigned char* senderPublicKey;
    unsigned char* receiverPublicKey;
    std::string domain;
    std::string asset;
    long long makotos;  // TODO: JS NUMBER range from -9007199254740992 to +9007199254740992 対応
    short int precision;

    MSGPACK_DEFINE(prevTxHash, hash, type, senderPublicKey, receiverPublicKey, domain, asset, makotos, precision, signature, timestamp);
};  // namespace transfer_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_
