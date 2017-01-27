javac -encoding UTF-8 $IROHA_HOME/smart_contract/Test/repository/*.java $IROHA_HOME/smart_contract/Test/Test.java
rm *.so

javah -jni repository.AccountRepository
javah -jni repository.AssetRepository


