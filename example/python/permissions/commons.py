#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import irohalib
import primitive_pb2
import binascii
from time import time


def now():
    return int(time() * 1000)


def all_permissions():
    return [
        primitive_pb2.can_append_role,
        primitive_pb2.can_create_role,
        primitive_pb2.can_detach_role,
        primitive_pb2.can_add_asset_qty,
        primitive_pb2.can_subtract_asset_qty,
        primitive_pb2.can_add_peer,
        primitive_pb2.can_add_signatory,
        primitive_pb2.can_remove_signatory,
        primitive_pb2.can_set_quorum,
        primitive_pb2.can_create_account,
        primitive_pb2.can_set_detail,
        primitive_pb2.can_create_asset,
        primitive_pb2.can_transfer,
        primitive_pb2.can_receive,
        primitive_pb2.can_create_domain,
        primitive_pb2.can_read_assets,
        primitive_pb2.can_get_roles,
        primitive_pb2.can_get_my_account,
        primitive_pb2.can_get_all_accounts,
        primitive_pb2.can_get_domain_accounts,
        primitive_pb2.can_get_my_signatories,
        primitive_pb2.can_get_all_signatories,
        primitive_pb2.can_get_domain_signatories,
        primitive_pb2.can_get_my_acc_ast,
        primitive_pb2.can_get_all_acc_ast,
        primitive_pb2.can_get_domain_acc_ast,
        primitive_pb2.can_get_my_acc_detail,
        primitive_pb2.can_get_all_acc_detail,
        primitive_pb2.can_get_domain_acc_detail,
        primitive_pb2.can_get_my_acc_txs,
        primitive_pb2.can_get_all_acc_txs,
        primitive_pb2.can_get_domain_acc_txs,
        primitive_pb2.can_get_my_acc_ast_txs,
        primitive_pb2.can_get_all_acc_ast_txs,
        primitive_pb2.can_get_domain_acc_ast_txs,
        primitive_pb2.can_get_my_txs,
        primitive_pb2.can_get_all_txs,
        primitive_pb2.can_get_blocks,
        primitive_pb2.can_grant_can_set_my_quorum,
        primitive_pb2.can_grant_can_add_my_signatory,
        primitive_pb2.can_grant_can_remove_my_signatory,
        primitive_pb2.can_grant_can_transfer_my_assets,
        primitive_pb2.can_grant_can_set_my_account_detail
    ]


def new_user(user_id):
    private_key = irohalib.IrohaCrypto.private_key()
    if user_id.lower().startswith('admin'):
        print('K{}'.format(private_key.decode('utf-8')))
    return {
        'id': user_id,
        'key': private_key
    }


def public_key_bytes(private_key_hex):
    """Derive public key from private and return its bytes representation"""
    public_key = irohalib.IrohaCrypto.derive_public_key(private_key_hex)
    return binascii.unhexlify(public_key)


def hex(generator):
    """
    Decorator for transactions' and queries generators.

    Allows preserving the type of binaries for Binary Testing Framework.
    """
    prefix = 'T' if generator.__name__.lower().endswith('tx') else 'Q'
    print('{}{}'.format(prefix, binascii.hexlify(generator().SerializeToString()).decode('utf-8')))
