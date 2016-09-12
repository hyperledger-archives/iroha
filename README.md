
# いろは(iroha)
![build status](https://circleci.com/gh/soramitsu/iroha.svg?style=shield&circle-token=80f2601e3bfb42d001e87728326659a0c96e0398)

 いろは(iroha) is ...

# Required
```
cmake(3.5.2)
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
