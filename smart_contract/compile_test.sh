rm $IROHA_HOME/smart_contract/instances/test/Test.class

javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/DomainRepository.java \
  $IROHA_HOME/smart_contract/instances/test/Test.java

# If recompiling repository/DomainRepository.java is needed, create C++ header by using following command.
# javah -cp $IROHA_HOME/smart_contract -d repository/ -jni repository.DomainRepository
