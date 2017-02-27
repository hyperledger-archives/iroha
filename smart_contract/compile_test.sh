rm $IROHA_HOME/smart_contract/instances/test/Test.class
rm $IROHA_HOME/smart_contract/instances/test/TestSimpleAsset.class

javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/Test.java

javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/Repository.java \
  $IROHA_HOME/smart_contract/repository/KeyConstants.java \
  $IROHA_HOME/smart_contract/instances/test/TestSimpleAsset.java

# If recompiling repository/Repository.java is needed, create C++ header by using following command.
# javah -cp $IROHA_HOME/smart_contract -d repository/ -jni repository.Repository
