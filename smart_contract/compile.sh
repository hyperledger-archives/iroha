rm *.so
# Account Repository
javac -encoding UTF-8 repository/AccountRepository.java
javah -jni repository.AccountRepository

g++ -fPIC -shared -std=c++1y \
  -o libAccountRepository.so \
  -Wl,-rpath,/usr/local/lib \
  -lprotobuf \
  -L${IROHA_HOME}/build/lib \
  ${IROHA_HOME}/build/lib/libworld_state_repo_with_level_db.a \
  ${IROHA_HOME}/build/lib/libdatetime.a \
  ${IROHA_HOME}/build/lib/libdomain_repository.a \
  -lworld_state_repo_with_level_db \
  -ldatetime \
  -ldomain_repository \
  -I/usr/lib/jvm/java-1.8.0-openjdk-amd64/include \
  -I/usr/lib/jvm/java-1.8.0-openjdk-amd64/include/linux/ \
  -D_GLIBCXX_USE_CXX11_ABI=0 \
  repository/account_repository_jni.cpp

execstack -c *.so

# For Test
cd Test; ./compile.sh
