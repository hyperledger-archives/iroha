

#include "consensus_event.hpp"

namespace event{
    
    template<typename T>
    using Transaction = transaction::Transaction<T>;
    template<typename T>
    using Transfer = command::Transfer<T>;
    template<typename T>
    using Add = command::Add<T>;

    template<typename T>
    using Update = command::Update<T>;

    using Type = json_parse::Type;
    using Object = json_parse::Object;

    template <>
    ConsensusEvent<Transaction<Transfer<object::Asset>>>::ConsensusEvent(
        Object obj
    ):
        Transaction(obj.dictSub["transaction"])
    {
        order = obj.dictSub["order"].integer;
        std::vector<Object> eventSigs = obj.dictSub["eventSignatures"].listSub;
        std::cout << eventSigs.size() << std::endl;
        for(auto&& sig : eventSigs){
            std::cout <<"Oh "<< sig.dictSub["publicKey"].str << " " << sig.dictSub["signature"].str << std::endl;
            if(
                sig.dictSub["publicKey"].str != "" &&
                sig.dictSub["signature"].str != ""
            ){
                _eventSignatures.push_back(eventSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
            }
        }
    }

    template <>
    ConsensusEvent<Transaction<Add<object::Asset>>>::ConsensusEvent(
        Object obj
    ):
        Transaction(obj.dictSub["transaction"])
    {
        order = obj.dictSub["order"].integer;
        std::vector<Object> eventSigs = obj.dictSub["eventSignatures"].listSub;
        for(auto&& sig : eventSigs){
            _eventSignatures.push_back(eventSignature(sig.dictSub["publicKey"].str, sig.dictSub["signature"].str));
        }
    }

    template <>
    ConsensusEvent<Transaction<Add<object::Domain>>>::ConsensusEvent(
        Object obj
    ):
        Transaction(obj)
    {
        order = obj.dictSub["order"].floating;
        std::vector<Object> eventSigs = obj.dictSub["eventSignatures"].listSub;
        for(auto&& sig : eventSigs){
            _eventSignatures.push_back(eventSignature(sig.dictSub["publicKey"].str,sig.dictSub["signature"].str));
        }
    }

    template <>
    void ConsensusEvent<Transaction<Update<object::Asset>>>::execution(){
        logger::info("execution","update! Asset");
    }
    template <>
    void ConsensusEvent<Transaction<Add<object::Asset>>>::execution(){
        logger::info("execution","add! Asset");
    }
    template <>
    void ConsensusEvent<Transaction<Add<object::Domain>>>::execution(){
        logger::info("execution","add! Asset");
    }

};