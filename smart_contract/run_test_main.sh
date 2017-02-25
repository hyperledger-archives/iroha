# should be configurable
rm -rf /tmp/iroha_ledger/

java -Djava.class.path=$IROHA_HOME/smart_contract \
     -Djava.library.path=$IROHA_HOME/build/lib \
     test.Test
