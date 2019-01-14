#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import irohalib
import commons
import primitive_pb2

admin = commons.new_user('admin@test')
alice = commons.new_user('alice@test')
iroha = irohalib.Iroha(admin['id'])


@commons.hex
def genesis_tx():
    test_permissions = [primitive_pb2.can_get_domain_acc_ast]
    genesis_commands = commons.genesis_block(admin, alice, test_permissions, multidomain=True)
    irohalib.IrohaCrypto.sign_transaction(tx, admin['key'])
    return tx


@commons.hex
def account_assets_query():
    query = iroha.query('GetAccountAssets', creator_account=alice['id'], account_id=admin['id'])
    irohalib.IrohaCrypto.sign_query(query, alice['key'])
    return query
