/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "../../util/logger.hpp"

#include "account.hpp"
#include "simple_asset.hpp"
#include "asset.hpp"
#include "domain.hpp"
#include "message.hpp"
//#include "peer.hpp"


namespace object {

	namespace detail {
	    template<
	    	typename T,
	    	typename... Args,
	    	template<typename U> typename AllocatorType = std::allocator
    	>
	    static T* allocateObject(Args&& ... args) {
	    	constexpr int NumAlloc = 1;
	       	AllocatorType<T> alloc;
	    	auto deleter = [&NumAlloc, &alloc](T* object) {
	        	alloc.deallocate(object, NumAlloc);
	        };
	        std::unique_ptr<T, decltype(deleter)> object(alloc.allocate(NumAlloc), deleter);
	        alloc.construct(object.get(), std::forward<Args>(args)...);

	        if (object == nullptr) {
	        	logger::fatal("model object") << "cannot allcoate object";
	        	exit(EXIT_FAILURE);
	        }

	        return object.release();
	    }
	}

    // There is kind of Currency, Asset,Domain,Account,Message and Peer. Associate SmartContract with Asset.
    Object::Object() = default;	// ctor for ValueT::null

    Object::Object(ObjectValueT t) {
        switch (t) {              

            case ObjectValueT::simpleAsset: {
                simpleAsset = detail::allocateObject<SimpleAsset>();
                break;
            }

            case ObjectValueT::asset: {
                asset = detail::allocateObject<Asset>();
                break;
            }

            case ObjectValueT::domain: {
                domain = detail::allocateObject<Domain>();
                break;
            }

            case ObjectValueT::account: {
                account = detail::allocateObject<Account>();
                break;
            }

            case ObjectValueT::message: {
                message = detail::allocateObject<Message>();
                break;
            }
            /*
            case ObjectValueT::peer: {
                peer = detail::allocateObject<Peer>();
            	break;
            }
            */
            case ObjectValueT::null: {
                break;
            }

            default: {
               	logger::fatal("model object") << "Unexpected ValueT: " << static_cast<std::uint8_t>(t);
               	exit(EXIT_FAILURE);
            }
        }
    }

    Object::Object(const SimpleAsset& rhs) {
        simpleAsset = detail::allocateObject<SimpleAsset>(rhs);
    }

    Object::Object(const Asset& rhs) {
        asset = detail::allocateObject<Asset>(rhs);
    }

    Object::Object(const Domain& rhs) {
        domain = detail::allocateObject<Domain>(rhs);
    }

    Object::Object(const Account& rhs) {
        account = detail::allocateObject<Account>(rhs);
    }

    Object::Object(const Message& rhs) {
        message = detail::allocateObject<Message>(rhs);
    }
    /*
    Object::Object(const Peer& rhs) {
        peer = detail::allocateObject<Peer>(rhs);
    }
    */
}
