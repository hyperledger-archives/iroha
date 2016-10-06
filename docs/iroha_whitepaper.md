#Iroha v1.0#

The following is a specification for Iroha 1.0. Many parts are still in development and have not yet been implemented.

---

## 1. Overview

Iroha is a simple and modularized distributed ledger platform.

### 1.1. Relationship to Fabric and Sawtooth Lake

It is our vision that in the future Hyperledger will consist less of disjointed projects and more of coherent libraries of components that can be selected and installed in order to run a Hyperledger network. Towards this end, it is the goal of Iroha to eventually provide the following encapsulated C++ components that other projects (particularly in Hyperledger) can use:

* Sumeragi consensus library
* Ed25519 digital signature library
* SHA-3 hashing library
* Iroha transaction serialization library
* P2P broadcast library
* API server library

### 1.2. Mobile and web libraries

## 2. System architecture

### 2.1. P2P Network

Generally, 3*f*+1 nodes are needed to tolerate *f* Byzantine nodes in the network (albeit some consensus algorithms have higher node requirements).

The following node types are considered:

* Client
* Validating peers
* Normal peer

### 2.2. Cryptography

Iroha aims to provide modular cryptographic functionality. At the present state, the default cryptographic library provides Ed25519 digital signatures and verification, using SHA-3.

### 2.3. Chaincode

### 2.4. Domains and assets



### 2.5. Transactions

Iroha supports the chaincode lifecycle transactions:

* Chaincode deploy
* Chaincode invoke
* Chaincode update
* Chaincode deprecate

Transaction types are as follows:

* Domain registration
* Asset registration
* Transfer

Arbitrary data can be stored using the following:

* Message blob

Additionally, the following two transaction types take as input (i.e., "wrap") one of the above transaction types:

* Multisignature
* Interledger (i.e., cross-chain)


### 2.6. Consensus

Iroha introduces a Byzantine Fault Tolerant consensus algorithm called Sumeragi. It is heavily inspired by the B-Chain algorithm:

Duan, S., Meling, H., Peisert, S., & Zhang, H. (2014). Bchain: Byzantine replication with high throughput and embedded reconfiguration. In International Conference on Principles of Distributed Systems (pp. 91-106). Springer.

Consensus in Sumeragi is performed on individual transactions and on the global state resulting from the application of the transaction. When a validating peer receives a transaction over the network, it performs the following steps in order:

* validate the signature (or signatures, in the case of multisignature transactions) of the transaction
* validate the contents of the transaction, where applicable (e.g., for transfer transactions, is the balance non-negative)
* temporarily apply the transaction to the ledger; this involves updating the Merkle root of the global state
* sign the updated Merkle root and the hash of the transaction contents
* broadcast the tuple `(signed Merkle root, tx hash)`


### 2.7. Data synchronization and retrieval

The state with the Merkle root that has 2*f*+1 signatures of validating servers is the most advanced state.

## Appendix

### A.1. Developing for Iroha

To ease development, Iroha makes use of the Python Fabric library (not to be confused with Hyperledger Fabric, the Python Fabric library enables running scripted commands on local and remote servers).