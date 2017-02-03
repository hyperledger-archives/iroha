cd $IROHA_HOME
rm -rf build
mkdir build
cd build
cmake ..
make
cp $IROHA_HOME/core/infra/crypto/libhash.a $IROHA_HOME/build/lib
make
cd test_bin
./java_vm_test
cd $IROHA_HOME/smart_contract
