#!/bin/bash
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestInvocation.java \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestAccount.java \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestAsset.java \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestSimpleAsset.java