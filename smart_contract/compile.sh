#javac -encoding UTF-8 \
#  test/repository/account/AccountRepository.java \
#  test/repository/asset/AssetRepository.java \
#  test/Test.java

#javah -cp $IROHA_HOME/smart_contract \
#  -d test/repository/account \
#  -jni test.repository.account.AccountRepository

#javah -cp $IROHA_HOME/smart_contract \
#  -d test/repository/asset \
#  -jni test.repository.asset.AssetRepository

javac -encoding UTF-8 \
  test/repository/DomainRepository.java \
  test/Test.java

javah -cp $IROHA_HOME/smart_contract \
  -d test/repository/ \
  -jni test.repository.DomainRepository
