rm $IROHA_HOME/smart_contract/Test/Test.class

javac -encoding UTF-8 \
  $IROHA_HOME/smart_contract/repository/DomainRepository.java \
  $IROHA_HOME/smart_contract/Test/Test.java

#javah -cp $IROHA_HOME/smart_contract -d repository/ -jni repository.DomainRepository
