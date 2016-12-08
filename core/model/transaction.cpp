

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
        senderPubkey(obj.dictSub["senderPublicKey"].str),
        hash(obj.dictSub["hash"].str)
    {
        
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
        senderPubkey(senderPubkey),
        Transfer(
            senderPubkey,
            receiverPubkey,
            name,
            value
        )
    {}

    template <>
    Transaction<Add<object::Asset>>::Transaction(
        const std::string& senderPubkey,
        const std::string& domain,
        const std::string& name,
        const unsigned long long& value,
        const unsigned int& precision
    ):
        senderPubkey(senderPubkey),
        Add(
            domain,
            name,
            value,
            precision
        )    
    {}

    template <>
    Transaction<Add<object::Domain>>::Transaction(
        const std::string& senderPubkey,
        const std::string& ownerPublicKey,
        const std::string& name
    ):
        senderPubkey(senderPubkey),
        Add(
            ownerPublicKey,
            name
        )    
    {}

    template <>
    Transaction<Update<object::Asset>>::Transaction(
        const std::string& ownerPublicKey,
        const std::string& name,
        const unsigned long long& value
    ):
        senderPubkey(ownerPublicKey),
        Update(
                ownerPublicKey,
                name,
                value
        )
    {}


    template <>
    void Transaction<Update<object::Asset>>::execution(){
        logger::info("execution","update! Asset "+ ownerPublicKey);
        repository::asset::update(ownerPublicKey, "sample", std::to_string(value));
    }
    template <>
    void Transaction<Add<object::Asset>>::execution(){
        logger::info("execution","add! Asset");
    }
    template <>
    void Transaction<Add<object::Domain>>::execution(){
        logger::info("execution","add! Domain");
    }
    template <>
    void Transaction<Transfer<object::Asset>>::execution(){
        logger::info("execution","Transfer! Asset");
    }


}