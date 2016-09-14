#ifndef CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_
#define CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_

#include <msgpack.hpp>

namespace SignatoryTransaction {
    MSGPACK_DEFINE(hash, type, senderPublicKey, receiverPublicKey, domain, asset, makotos, precision);
};

#endif  // CORE_DOMAIN_SIGNATORYTRANSACTION_HPP_
