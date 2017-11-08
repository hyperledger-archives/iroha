/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
#define IROHA_SHARED_MODEL_PROTO_COMMAND_HPP

#include "commands.pb.h"
#include "interfaces/commands/command.hpp"
#include "backend/protobuf/commands/proto_add_asset_quantity.hpp"

#include <boost/serialization/variant.hpp>

void load(
    const iroha::protocol::Command & ar,
    shared_model::interface::Command::CommandVariantType & v
);

namespace shared_model {
  namespace proto {
    class Command : public interface::Command {
     private:
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;
     public:
      explicit Command(const iroha::protocol::Command &command) {
        load(command, variant_);
      }

      const CommandVariantType &get() const override {
        return variant_;
      }

      using ProtoCommandVariantType = boost::variant<w<AddAssetQuantity>>;
      using ProtoCommandListType = ProtoCommandVariantType::types;

     private:
      CommandVariantType variant_;
    };
  } // namespace proto
} // namespace shared_model

template<class S>
struct variant_impl {

  struct load_null {
    template<class Archive, class V>
    static void invoke(
        Archive & /*ar*/,
        int /*which*/,
        V & /*v*/
    ){}
  };

  struct load_impl {
    template<class Archive, class V>
    static void invoke(
        Archive & ar,
        int which,
        V & v
    ){
      if(which == 0){
        // note: A non-intrusive implementation (such as this one)
        // necessary has to copy the value.  This wouldn't be necessary
        // with an implementation that de-serialized to the address of the
        // aligned storage included in the variant.
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::front<S>::type head_type;
        head_type value;
        value = ar;
        v = value;
        return;
      }
      typedef BOOST_DEDUCED_TYPENAME boost::mpl::pop_front<S>::type type;
      variant_impl<type>::load(ar, which - 1, v);
    }
  };

  template<class Archive, class V>
  static void load(
      Archive & ar,
      int which,
      V & v
  ){
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<boost::mpl::empty<S>,
                                                       boost::mpl::identity<load_null>,
                                                       boost::mpl::identity<load_impl>
    >::type typex;
    typex::invoke(ar, which, v);
  }

};

void load(
    const iroha::protocol::Command & ar,
    shared_model::interface::Command::CommandVariantType & v
){
  int which;
  typedef BOOST_DEDUCED_TYPENAME shared_model::proto::Command::ProtoCommandListType types;
  which = ar.command_case() - 1;
  if(which >=  boost::mpl::size<types>::value)
    // this might happen if a type was removed from the list of variant types
    boost::serialization::throw_exception(
        boost::archive::archive_exception(
            boost::archive::archive_exception::unsupported_version
        )
    );
  variant_impl<types>::load(ar, which, v);
}

#endif //IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
