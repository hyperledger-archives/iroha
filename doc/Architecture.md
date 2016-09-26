# Architecture

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

