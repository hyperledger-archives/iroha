

#include "convertor.hpp"

namespace convertor{

    using namespace transaction;
    using namespace command;
    using namespace event;

    namespace detail{

        Event::Asset encodeObject(object::Asset aAsset){
            Event::Asset asset;
            asset.set_domain(aAsset.domain);
            asset.set_name(aAsset.name);
            asset.set_value(aAsset.value);
            asset.set_precision(aAsset.precision);
            return asset;
        }

        Event::Domain encodeObject(object::Domain aDomain){
            Event::Domain domain;
            domain.set_name(aDomain.name);
            domain.set_ownerpublickey(aDomain.ownerPublicKey);
            return domain;
        }
        // ====== decode ======

        object::Asset decodeObject(Event::Asset aAsset){
            auto domain = aAsset.domain();
            auto name = aAsset.name();
            return object::Asset(
                    std::move(domain),
                    std::move(name),
                    aAsset.value(),
                    aAsset.precision()
            );
        }

        template <>
        ConsensusEvent<Transaction<Transfer<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx){
            auto name = tx.asset().name();
            auto sender = tx.senderpubkey();
            auto issuer = tx.senderpubkey();
            auto receiver = tx.receivepubkey();
            return
                ConsensusEvent<
                    Transaction<
                        Transfer<object::Asset>
                    >
                >(
                    std::move(issuer),
                    std::move(sender),
                    std::move(receiver),
                    std::move(name),
                    tx.asset().value()
                );
        }

        template <>
        ConsensusEvent<Transaction<Add<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx){
            auto name = tx.asset().name();
            auto domain = tx.asset().domain();
            auto issuer = tx.senderpubkey();
            return
                ConsensusEvent<
                    Transaction<
                        Add<object::Asset>
                    >
                >(
                    std::move(issuer),
                    std::move(domain),
                    std::move(name),
                    tx.asset().value(),
                    tx.asset().precision()
                );
        }

        template <>
        ConsensusEvent<Transaction<Update<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx){
            auto name = tx.asset().name();
            auto issuer = tx.senderpubkey();
            auto owner = tx.senderpubkey();
            return
                ConsensusEvent<
                    Transaction<
                            Update<object::Asset>
                    >
                >(
                    std::move(issuer),
                    std::move(owner),
                    std::move(name),
                    tx.asset().value()
                );
        }

    }

};
