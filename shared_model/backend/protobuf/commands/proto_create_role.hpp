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

#include <boost/assign/list_inserter.hpp>
#include <boost/bimap.hpp>
#include "interfaces/commands/create_role.hpp"
#include "interfaces/permissions.hpp"

#ifndef IROHA_PROTO_CREATE_ROLE_HPP
#define IROHA_PROTO_CREATE_ROLE_HPP

namespace shared_model {
  namespace proto {
    class CreateRole final : public interface::CreateRole {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit CreateRole(const iroha::protocol::Command &command)
          : CreateRole(command.create_role()) {
        if (not command.has_create_role()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument("Object does not contain create_asset");
        }

        boost::assign::insert(pb_role_map_)
            // Can Get My Account Assets
            (iroha::protocol::RolePermission::can_get_my_acc_ast,
             interface::can_get_my_acc_ast)
            // Can Get My Signatories
            (iroha::protocol::RolePermission::can_get_my_signatories,
             interface::can_get_my_signatories)
            // Can set quorum
            (iroha::protocol::RolePermission::can_set_quorum,
             interface::can_set_quorum)
            // Can get my account transactions
            (iroha::protocol::RolePermission::can_get_my_acc_txs,
             interface::can_get_my_acc_txs)
            // Can get my account
            (iroha::protocol::RolePermission::can_get_my_account,
             interface::can_get_my_account)
            // Can get all account assets
            (iroha::protocol::RolePermission::can_get_all_acc_ast,
             interface::can_get_all_acc_ast)
            // Can get all account asset transactions
            (iroha::protocol::RolePermission::can_get_all_acc_ast_txs,
             interface::can_get_all_acc_ast_txs)
            // Can Get all account transactions
            (iroha::protocol::RolePermission::can_get_all_acc_txs,
             interface::can_get_all_acc_txs)
            // Can get all account
            (iroha::protocol::RolePermission::can_get_all_accounts,
             interface::can_get_all_accounts)
            // Can remove signatory
            (iroha::protocol::RolePermission::can_remove_signatory,
             interface::can_remove_signatory)
            // Can add signatory
            (iroha::protocol::RolePermission::can_add_signatory,
             interface::can_add_signatory)
            // Can create domain
            (iroha::protocol::RolePermission::can_create_domain,
             interface::can_create_domain)
            // Can create account
            (iroha::protocol::RolePermission::can_create_account,
             interface::can_create_account)
            // Can set quorum
            (iroha::protocol::RolePermission::can_set_quorum,
             interface::can_set_quorum)
            // Can add peer
            (iroha::protocol::RolePermission::can_add_peer,
             interface::can_add_peer)
            // Can add asset quantity
            (iroha::protocol::RolePermission::can_add_asset_qty,
             interface::can_add_asset_qty)
            // Can append role
            (iroha::protocol::RolePermission::can_append_role,
             interface::can_append_role)
            // Can create asset
            (iroha::protocol::RolePermission::can_create_asset,
             interface::can_create_asset)
            // Can create role
            (iroha::protocol::RolePermission::can_create_role,
             interface::can_create_role)
            // Can get all signatories
            (iroha::protocol::RolePermission::can_get_all_signatories,
             interface::can_get_all_signatories)
            // Can get my account assets transactions
            (iroha::protocol::RolePermission::can_get_my_acc_ast_txs,
             interface::can_get_my_acc_ast_txs)
            // Can transfer
            (iroha::protocol::RolePermission::can_transfer,
             interface::can_transfer)
            // Can receive
            (iroha::protocol::RolePermission::can_receive,
             interface::can_receive)
            // Can read assets
            (iroha::protocol::RolePermission::can_read_assets,
             interface::can_read_assets)
            // Can grant set quorum
            (iroha::protocol::RolePermission::can_grant_set_quorum,
             interface::can_grant + interface::can_set_quorum)
            // Can grant remove signatory
            (iroha::protocol::RolePermission::can_grant_remove_signatory,
             interface::can_grant + interface::can_remove_signatory)
            // Can grant add signatory
            (iroha::protocol::RolePermission::can_grant_add_signatory,
             interface::can_grant + interface::can_add_signatory)
            // Can get roles
            (iroha::protocol::RolePermission::can_get_roles,
             interface::can_get_roles);

        boost::assign::insert(pb_grant_map_)
            // Can add my signatory
            (iroha::protocol::GrantablePermission::can_add_my_signatory,
             interface::can_add_signatory)
            // Can remove my signatory
            (iroha::protocol::GrantablePermission::can_remove_my_signatory,
             interface::can_remove_signatory)
            // Can set my quorum
            (iroha::protocol::GrantablePermission::can_set_my_quorum,
             interface::can_set_quorum);
      }

      const interface::types::RoleIdType &roleName() const override {
        return create_role_.role_name();
      }

      const PermissionsType &rolePermissions() const override {
        return role_permissions_.get();
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override { return new CreateRole(create_role_); }

     private:
      // ----------------------------| private API |----------------------------
      explicit CreateRole(const iroha::protocol::CreateRole &create_role)
          : create_role_(create_role),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }),
            role_permissions_([this] {
              std::set<std::string> perms;

              std::for_each(
                  create_role_.permissions().begin(),
                  create_role_.permissions().end(),
                  [&perms, this](auto perm) {
                    auto it = this->pb_role_map_.left.find(
                        static_cast<iroha::protocol::RolePermission>(perm));
                    if (it != this->pb_role_map_.left.end()) {
                      perms.insert(it->second);
                    }
                  });
              return perms;
            }) {}

      iroha::protocol::CreateRole create_role_;
      Lazy<crypto::Hash> hash_;
      Lazy<PermissionsType> role_permissions_;

      boost::bimap<iroha::protocol::RolePermission, std::string> pb_role_map_;
      boost::bimap<iroha::protocol::GrantablePermission, std::string>
          pb_grant_map_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_ROLE_HPP
