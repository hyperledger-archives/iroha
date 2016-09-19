
# いろは(iroha)
![build status](https://circleci.com/gh/soramitsu/iroha.svg?style=shield&circle-token=80f2601e3bfb42d001e87728326659a0c96e0398)

 いろは(iroha) is ...

 ![alt tag](https://github.com/soramitsu/iroha/raw/feature/sumeragi/LGTM.gif)

# Required
```
cmake(3.5.2)
```

# Prepare
```
sudo apt -y install default-jdk
sudo apt -y install default-jre 
sudo apt -y install libssl-dev
```

# Env
```
# if OSX
export JAVA_HOME=$(/usr/libexec/java_home)
# if ubuntu
export JAVA_HOME=$(readlink -f $(which java) | sed -e "s:/jre/bin/java::")

export IROHA_HOME=$(pwd)/iroha
```

# Usage
```
cd $IROHA_HOME
git submodule init 
git submodule update
cd core/vendor/ed25519; make
cd $IROHA_HOME
cd code/vendor/leveldb; make
cd $IROHA_HOME
cd core/vendor/msgpack-c; cmake -DMSGPACK_CXX11=ON .; sudo make install
mkdir build
cd build
cmake ..
make 
../test.sh
```
