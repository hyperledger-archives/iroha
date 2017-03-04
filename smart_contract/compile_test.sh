#!/bin/bash
mkdir -p $IROHA_HOME/smart_contract/java_tests/instances/test
mkdir -p $IROHA_HOME/smart_contract/java_tests/instances/sample_rating
mkdir -p $IROHA_HOME/smart_contract/java_tests/repository

javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestInvocation.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestInvocation.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestAccount.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestAccount.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestAsset.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestAsset.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestSimpleAsset.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestSimpleAsset.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestDomain.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestDomain.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestPeer.java \
&& \
mv $IROHA_HOME/smart_contract/instances/test/TestPeer.class $IROHA_HOME/smart_contract/java_tests/instances/test/ \
&& \
javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/sample_rating/WeightedRate.java \
&& \
mv $IROHA_HOME/smart_contract/instances/sample_rating/WeightedRate.class $IROHA_HOME/smart_contract/java_tests/instances/sample_rating/ \
&& \
mv $IROHA_HOME/smart_contract/repository/Repository.class $IROHA_HOME/smart_contract/java_tests/repository
