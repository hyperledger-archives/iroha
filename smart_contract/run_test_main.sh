#!/bin/bash
java -Djava.class.path=$IROHA_HOME/smart_contract/ \
     -Djava.library.path=$IROHA_HOME/build/lib \
     instances.test.Test
