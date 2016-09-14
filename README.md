
# いろは(iroha)
![build status](https://circleci.com/gh/soramitsu/iroha.svg?style=shield&circle-token=80f2601e3bfb42d001e87728326659a0c96e0398)

 いろは(iroha) is ...

 ![alt tag](https://github.com/soramitsu/iroha/raw/feature/sumeragi/LGTM.gif)


準備：

add-apt-repository ppa:webupd8team/java
apt-get update
apt-get install oracle-java8-installer
apt-get install g++
apt-get install make
apt-get install cmake
apt-get install build-essential
apt-get install zlib1g-dev libssl-dev

git submodule init
git submodule update

AERON:
./gradlew
./gradlew
./cppbuild/cppbuild

mkdir -p cppbuild/Debug
cd cppbuild/Debug
cmake ../..
cmake --build . --clean-first
ctest

yaml-cpp:
mkdir build
cd build
cmake ..
make
sudo make install

# Required
```
cmake(3.5.2)
```

# Prepare
```
export JAVA_HOME=$(/usr/libexec/java_home)
```
# Usage
```
git submodule init 
git submodule update
cd core/vendor/ed25519; make
cd core/vendor/msgpack-c; cmake -DMSGPACK_CXX11=ON .; sudo make install
mkdir build
cd build
cmake ..
make 
../test.ch
```
