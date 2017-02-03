

#include <vector>
#include <tuple>
#include "convertor.hpp"
#include "../../model/objects/base_object.hpp"

namespace convertor{

    using namespace transaction;
    using namespace command;
    using namespace event;

    Event::Asset encodeObject(object::Asset objAsset) {
        Event::Asset res;
        res.set_domain(objAsset.domain);
        res.set_name(objAsset.name);
        // TODO: res.set_value(aAsset.value);
        return res;
    }

    Event::Domain encodeObject(object::Domain objDomain) {
        Event::Domain res;
        res.set_name(objDomain.name);
        res.set_ownerpublickey(objDomain.ownerPublicKey);
        return res;
    }

    Event::Account encodeObject(object::Account objAccount) {
        Event::Account res;
        res.set_name(objAccount.name);
        res.set_publickey(objAccount.publicKey);
        for(auto&& e: objAccount.assets){
            Event::Asset protoAsset;
            res.set_name(std::get<0>(e));
            //protoAsset.set_value(std::get<1>(e));
            res.add_assets()->CopyFrom(protoAsset);
        }
        return res;
    }
    // ====== decode ======

    object::Object decodeObject(Event::Object o) {
        switch (o.value_case()) {
            /*
            case Event::Object::kValueStringFieldNumber:
                return object::Object(o.valuestring());
            case Event::Object::kValueIntFieldNumber:
                return object::Object(o.valueint());
            case Event::Object::kValueBooleanFieldNumber:
                return object::Object(o.valueboolean());
            case Event::Object::kValueDoubleFieldNumber:
                return object::Object(o.valuedouble());
                */

                /*
            case Event::Object::kValueSimpleAssetFieldNumber:
                return object::Object(o.valuesimpleasset());
            case Event::Object::kValueAssetFieldNumber:
                return object::Object(o.valueasset());
            case Event::Object::kValueDomainFieldNumber:
                return object::Object(o.valuedomain());
            case Event::Object::kValuePeerFieldNumber:
                return object::Object(o.valuepeer());
                */
            default:

                logger::fatal("convertor::decodeObject") << "Undefined Event::Object type";
                exit(EXIT_FAILURE);

        }

        logger::fatal("convertor::decodeObject") << "No arrival error";
        exit(EXIT_FAILURE);
    }

    namespace detail {
        std::unordered_map<std::string, object::Object> decodeMapSecondProto(std::unordered_map<std::string, Event::Object>&& aMap) {
            std::unordered_map<std::string, object::Object> res;
            for (auto&& e: aMap) {
                res[e.first] = ::convertor::decodeObject(std::move(e.second));
            }
            return res;
        }
    }

    object::Asset decodeObject(Event::Asset protoAsset) {
        auto domain = protoAsset.domain();
        auto name   = protoAsset.name();
//        auto objAssetValue = detail::decodeObject(protoAsset.value());

        std::unordered_map<std::string, Event::Object> standardMap(
            protoAsset.value().begin(),
            protoAsset.value().end()
        );

        auto objAssetValue = detail::decodeMapSecondProto(std::move(standardMap));

        return object::Asset(
            std::move(domain),
            std::move(name),
            std::move(objAssetValue)
        );
    }

    object::Account decodeObject(Event::Account aAccount) {
        auto publicKey = aAccount.publickey();
        auto name = aAccount.name();
        std::vector<std::tuple<std::string, std::int64_t>> assets;
        for(const Event::Asset& as: aAccount.assets()){
            assets.emplace_back(as.name(), static_cast<std::int64_t>(1)); //TODO: Replace with as.value()));
        }
        return object::Account(
            std::move(publicKey),
            std::move(name),
            std::move(assets)
        );
    }

    template <>
    ConsensusEvent<Transaction<Transfer<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx) {

        auto issuer     = tx.senderpubkey();
        auto sender     = tx.senderpubkey();
        auto receiver   = tx.receivepubkey();

        const auto& protoAsset = tx.asset();
        auto name       = protoAsset.name();

        std::unordered_map<std::string, Event::Object> standardMap(
            protoAsset.value().begin(),
            protoAsset.value().end()
        );

        auto objAssetValue = detail::decodeMapSecondProto(std::move(standardMap));

        auto res = ConsensusEvent<
                Transaction<
                    Transfer<object::Asset>
                >
            >(
                std::move(issuer),
                std::move(sender),
                std::move(receiver),
                std::move(name),
                std::move(objAssetValue)
            );
        res.timestamp = tx.timestamp();
        return res;
    }

    template <>
    ConsensusEvent<Transaction<Add<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx) {
        auto name = tx.asset().name();
        auto domain = tx.asset().domain();
        auto issuer = tx.senderpubkey();

        auto& protoAsset = tx.asset();

        std::unordered_map<std::string, Event::Object> standardMap(
            protoAsset.value().begin(),
            protoAsset.value().end()
        );

        auto objAssetValue = detail::decodeMapSecondProto(std::move(standardMap));

        auto res = ConsensusEvent<
                Transaction<
                    Add<object::Asset>
                >
            >(
                std::move(issuer),
                std::move(domain),
                std::move(name),
                std::move(objAssetValue)
            );
        res.timestamp = tx.timestamp();
        return res;
    }

    template <>
    ConsensusEvent<Transaction<Add<object::Account>>> decodeTransaction2ConsensusEvent(Event::Transaction tx) {
        auto name = tx.account().name();
        auto publicKey = tx.account().publickey();
        std::vector<std::tuple<std::string,long>> assets;
        for(auto&& as: tx.account().assets()){
            assets.emplace_back(as.name(), 1);//as.value());
        }
        auto issuer = tx.senderpubkey();
        auto res = ConsensusEvent<
                Transaction<
                        Add<object::Account>
                >
            >(
                std::move(issuer),
                std::move(publicKey),
                std::move(name),
                std::move(assets)
            );
        res.timestamp = tx.timestamp();
        return res;
    }



    template <>
    ConsensusEvent<Transaction<Update<object::Asset>>> decodeTransaction2ConsensusEvent(Event::Transaction tx) {
        auto name = tx.asset().name();
        auto issuer = tx.senderpubkey();
        auto owner = tx.senderpubkey();
        auto res = ConsensusEvent<
                Transaction<
                        Update<object::Asset>
                >
            >(
                std::move(issuer),
                std::move(owner),
                std::move(name),
                1//tx.asset().value()
            );
        res.timestamp = tx.timestamp();
        return res;
    }

};