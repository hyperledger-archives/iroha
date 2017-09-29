../build/bin/iroha-cli --grpc --json_transaction CreateDomain.json
../build/bin/iroha-cli --grpc --json_transaction CreateAsset.json
../build/bin/iroha-cli --grpc --json_transaction CreateAccount-alice.json
../build/bin/iroha-cli --grpc --json_transaction CreateAccount-bob.json
../build/bin/iroha-cli --grpc --json_transaction AddSignatory-bob.json
../build/bin/iroha-cli --grpc --json_transaction AddAssetQuantity-alice.json
../build/bin/iroha-cli --grpc --json_transaction AddAssetQuantity-bob.json
../build/bin/iroha-cli --grpc --json_transaction TransferAsset.json