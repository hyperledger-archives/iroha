javac -encoding UTF-8 $IROHA_HOME/smart_contract/Test/repository/*.java $IROHA_HOME/smart_contract/Test/Test.java
rm *.so

javah -classpath $IROHA_HOME/smart_contract/Test/ -jni repository.AccountRepository
javah -classpath $IROHA_HOME/smart_contract/Test/ -jni repository.AssetRepository


