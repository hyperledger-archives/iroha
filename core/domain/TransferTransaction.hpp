#ifndef CORE_DOMAIN_TRANSFERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSFERTRANSACTION_HPP_

#include "AbstractTransaction.hpp"
#include <msgpack.hpp>

namespace TransferTransaction {
    AbstractTransaction::TransactionType type;
    unsigned char* senderPublicKey;
    unsigned char* receiverPublicKey;
    std::string domain;
    std::string asset;
    long long makotos; // TODO: JS range from -9007199254740992 to +9007199254740992 対応
    short int precision;

    MSGPACK_DEFINE(hash, type, senderPublicKey, receiverPublicKey, domain, asset, makotos, precision);
};  // namespace TransferTransaction

#endif  // CORE_DOMAIN_TRANSFERTRANSACTION_HPP_
