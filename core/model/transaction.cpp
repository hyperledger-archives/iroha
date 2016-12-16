

#include "transaction.hpp"

namespace transaction{
    using command::Transfer;
    using command::Add;
    using command::Update;

    template <>
    Transaction<Transfer<object::Asset>>::Transaction(
        Object obj
    ):
        Transfer(
            obj.dictSub["command"]
        ),
        hash(obj.dictSub["hash"].str),
        senderPubkey(obj.dictSub["senderPublicKey"].str)
    {
        timestamp = obj.dictSub["timestamp"].integer;

        std::vector<Object> txSigs = obj.dictSub["txSignatures"].listSub;
        for(auto&& sig : txSigs){
            txSignatures.push_back(txSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
        }
    }

    template <>
    Transaction<Add<object::Asset>>::Transaction(
        Object obj
    ):
        Add(
            obj.dictSub["command"]
        )
    {
        senderPubkey = obj.dictSub["senderPublicKey"].str;
        hash = obj.dictSub["hash"].str;
        timestamp = obj.dictSub["timestamp"].integer;
        std::vector<Object> txSigs = obj.dictSub["txSignatures"].listSub;
        for(auto&& sig : txSigs){
            txSignatures.push_back(txSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
        }
    }

    template <>
    Transaction<Add<object::Domain>>::Transaction(
        Object obj
    ):
        Add(
            obj.dictSub["command"]
        )
    {
        senderPubkey = obj.dictSub["senderPublicKey"].str;
        hash = obj.dictSub["hash"].str;
        timestamp = obj.dictSub["timestamp"].integer;
        std::vector<Object> txSigs = obj.dictSub["txSignatures"].listSub;
        for(auto&& sig : txSigs){
            txSignatures.push_back(txSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
        }
    }
    template <>
    Transaction<Update<object::Asset>>::Transaction(
        Object obj
    ):
        Update(
            obj.dictSub["command"]
        )
    {
        senderPubkey = obj.dictSub["senderPublicKey"].str;
        hash = obj.dictSub["hash"].str;
        timestamp = obj.dictSub["timestamp"].integer;
        std::vector<Object> txSigs = obj.dictSub["txSignatures"].listSub;
        for(auto&& sig : txSigs){
            txSignatures.push_back(txSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
        }
    }

    template <>
    Transaction<Transfer<object::Asset>>::Transaction(
        const std::string& senderPubkey,
        const std::string& receiverPubkey,
        const std::string& name,
        const int& value
    ):
        timestamp(datetime::unixtime()),
        Transfer(
            senderPubkey,
            receiverPubkey,
            name,
            value
        ),
        senderPubkey(senderPubkey)
    {}

    template <>
    Transaction<Add<object::Asset>>::Transaction(
        const std::string& senderPubkey,
        const std::string& domain,
        const std::string& name,
        const unsigned long long& value,
        const unsigned int& precision
    ):
        timestamp(datetime::unixtime()),
        Add(
            domain,
            name,
            value,
            precision
        ),
        senderPubkey(senderPubkey)
    {}

    template <>
    Transaction<Add<object::Domain>>::Transaction(
        const std::string& senderPubkey,
        const std::string& ownerPublicKey,
        const std::string& name
    ):
        timestamp(datetime::unixtime()),
        Add(
            ownerPublicKey,
            name
        ),
        senderPubkey(senderPubkey)
    {}

    template <>
    Transaction<Update<object::Asset>>::Transaction(
        const std::string& ownerPublicKey,
        const std::string& name,
        const unsigned long long& value
    ):
        timestamp(datetime::unixtime()),
        senderPubkey(ownerPublicKey),
        Update(
                ownerPublicKey,
                name,
                value
        )
    {}

}