#!/usr/bin/env bash
echo node$1
iroha-cli --genesis_block --peers_address /data/peers.list
irohad --genesis_block genesis.block --config /data/config.sample --keypair_name node$1
