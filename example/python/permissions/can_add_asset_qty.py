#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import irohalib
import commons
import primitive_pb2

admin = commons.new_user('admin@test')
alice = commons.new_user('alice@test')


@commons.hex
def genesis_tx():
    iroha = irohalib.Iroha(admin['id'])
    peer = primitive_pb2.Peer()
    peer.address = '0.0.0.0:50541'
    peer.peer_key = commons.public_key_bytes(admin['key'])
    test_permissions = [primitive_pb2.can_add_asset_qty]
    tx = iroha.transaction([
        iroha.command('AddPeer', peer=peer),
        iroha.command('CreateRole', role_name='admin_role', permissions=commons.all_permissions()),
        iroha.command('CreateRole', role_name='test_role', permissions=test_permissions),
        iroha.command('CreateDomain', domain_id='test', default_role='test_role'),
        iroha.command('CreateAccount', account_name='admin', domain_id='test',
                      public_key=commons.public_key_bytes(admin['key'])),
        iroha.command('CreateAccount', account_name='alice', domain_id='test',
                      public_key=commons.public_key_bytes(alice['key'])),
        iroha.command('AppendRole', account_id=admin['id'], role_name='admin_role'),
        iroha.command('CreateAsset', asset_name='coin', domain_id='test', precision=2)
    ])
    irohalib.IrohaCrypto.sign_transaction(tx, admin['key'])
    return tx


@commons.hex
def add_asset_tx():
    iroha = irohalib.Iroha(alice['id'])
    tx = iroha.transaction([
        iroha.command('AddAssetQuantity', asset_id='coin#test', amount='5000.99')
    ])
    irohalib.IrohaCrypto.sign_transaction(tx, alice['key'])
    return tx
