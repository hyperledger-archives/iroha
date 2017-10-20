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

#ifndef IROHA_SHARED_MODEL_COMMAND_HPP
#define IROHA_SHARED_MODEL_COMMAND_HPP

#include <interfaces/visitor_apply_for_all.hpp>
#include <utility>
#include "boost/variant.hpp"
#include "interfaces/commands/add_asset_quantity.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/primitive.hpp"
#include "model/command.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Class provides commands container for all commands in system.
     * General note: this class is container for commands, not a base class, for
     * avoid this misunderstanding class should be final
     */
    class Command final : public Primitive<Command, iroha::model::Command> {
     public:
      Command(const Command &rhs) {
        this->command_variant = rhs.command_variant;
      }

      /// Shortcut type for polymorphic wrapper
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

     private:
      /**
       * Variant with wrapper on current concrete command
       * @return variant with concrete command
       */
      boost::variant<w<AddAssetQuantity>> command_variant;

     public:
      /**
       * @return const reference for attached variant with concrete commands
       */
      const auto get() const -> const decltype(command_variant) & {
        return command_variant;
      }

      /// Types of concrete commands, in attached variant
      using CommandsType = decltype(command_variant)::types;

      std::string toString() const override {
        return boost::apply_visitor(detail::ToStringVisitor(), command_variant);
      }

      Command *copy() const override { return new Command(*this); }

      OldModelType *makeOldModel() const {
        return boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType *>(), command_variant);
      }

      bool operator==(const Command &rhs) const override {
        return this->command_variant == rhs.command_variant;
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_COMMAND_HPP
