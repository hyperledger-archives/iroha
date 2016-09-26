# いろは(iroha)
![build status](https://circleci.com/gh/soramitsu/iroha.svg?style=shield&circle-token=80f2601e3bfb42d001e87728326659a0c96e0398)

いろは(iroha) is ...  
![alt tag](https://github.com/soramitsu/iroha/raw/feature/sumeragi/LGTM.gif)
  
# Architecture (Draft)

### Directory tree
```
.
├── build (for cmake)
├── config
├── peer
├── core
│   ├── connection
│   ├── consensus
│   ├── crypto
│   ├── domain
│   │   └── transactions
│   ├── infra
│   │   ├── connection
│   │   ├── crypto
│   │   ├── repository
│   │   ├── server
│   │   └── smart_contract
│   ├── repository
│   ├── server
│   ├── smart_contract
│   ├── util
│   ├── validation
│   └── vendor
├── doc
├── smart_contract
│   └── SampleCurrency_java
└── test
    ├── crypto
    └── smart_contract
```

#### config/

It contains config file. currently we use yaml, but JSON is good like as yaml.

#### peer/

It contains main.cpp

#### smart_contract/

It contains smart_contract logic what user defines.  
Currently use java virtual machine.

#### test/

There's few test for iroha. :bow:

#### core/

It contains main.

```
├── connection
├── consensus
├── crypto
├── domain
│   └── transactions
├── infra
│   ├── connection
│   ├── crypto
│   ├── repository
│   ├── server
│   └── smart_contract
│       └── jvm
├── repository
├── server
├── smart_contract
├── util
└── validation
```

I adopt a thought of **Domain Driven Development structure** as much as possible.

```
+--------------+
| web rest api |
+--------------+
      | 
+--------------+
|  controller  |--------+-----------------------------------------+
+--------------+        |                                         |
      |          +-------------+  +----------------+   +----------------------+
      |          | repository  |  | domain model   |   | service              |
      |          | (interface) |--|                |   |+---------++---------+|
      |          +-------------+  |+--------------+|   || crypto  ||validate ||
      |                 |         || transaction  ||   || base64  |+---------+|
      |                 |         |+--------------+|   || hash    |           |
      |                 |         || asset        ||   |+---------+           |
      |                 |         |+--------------+|   +----------------------+
      |                 |         |+--------------+|             
      |                 |         ||smart contract||              
+--------------+        |         |+--------------+|
|   consensus  |--------+         +----------------+
|              |
|+------------+|
|| messeging  ||
|+------------+|
+--------------+

+----------------------------------------------------------------------------+
|infrastructure                                                              |
|                                                                            |
|+------------++--------------++-------------++----------------++---------+  |
|| messaging  || web resr api || repository  || smart contract || crypto  |  |
||(use aeron )||  (use crow)  ||(use leveldb)|| (use java vm)  || ed25519 |  |
|+------------++--------------++-------------++----------------++---------+  |
+----------------------------------------------------------------------------+

```


#### core/connection (consensus layer)
It contains messaging function interface.
```C
  void initialize_peer( std::unordered_map<std::string, std::string> config);

  bool sendAll(std::string message);
  bool send(std::string to,std::string message);
  bool receive(std::function<void(std::string from,std::string message)> callback);
```

#### core/consensus (consensus layer)
It contains consensus algorithm.
  
#### core/crypto (service)
It contains electronic signature, base64, hash function interface...
  
#### core/model (domain)
It contains asset model, transaction logic. independent of infra knowledge.

#### core/infra (infra layer)
It contains some source depend on vendor libraries.
If any source depends on vendor libraries, it should be in infra. 
  
##### filename
basically, filename is `"function"_with_"lib name".cpp`
```
connection
 └── connection_with_aeron.cpp
repository
 └── world_state_repository_with_level_db.cpp
```

#### core/server (UI layer)
It contains server interface. currently
```
void server();
```

#### core/smart_contract (service)
It contains management virtual machine interface.
```
void initializeVM(std::string contractName);
void finishVM();    
void invokeFunction(
    std::string functionName,
      std::unordered_map<std::string, std::string> params);
```

#### core/util (other)
It contains logger, random, datetime, exception...

### Environment
```
JAVA_HOME  := java's jome
IROHA_HOME := iroha's root
```
  
## Requirement
```
cmake(3.5.2)
```
  
## Recommend
```
fabric3 (python library, not hyperledger/fabric)
```

## Installation
```
$ mkdir build
$ cmake ..
$ make
```
(in server)
or  
```
$ fab deploy
```
(in local)  
  
## Author

[MakotoTakemiya](https://github.com/takemiyamakoto)  
[MizukiSonoko](https://github.com/MizukiSonoko)

## License

Copyright 2016 Soramitsu Co., Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
