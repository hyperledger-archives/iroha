#Iroha v1.0#

The following is a specification for Iroha 1.0. Many parts are still in development and have not yet been implemented.

---

## 1. Overview

Iroha is a simple and modularized distributed ledger platform.

### 1.1. Relationship to Fabric and Sawtooth Lake

It is our vision that in the future Hyperledger will consist less of disjointed projects and more of coherent libraries of components that can be selected and installed in order to run a Hyperledger network.

## 2. System architecture

### 2.1. P2P Network

The following node types are considered:

* Client
* Validating peers
* Normal peer

### 2.1. Cryptography

Iroha aims to provide modular cryptographic functionality. At the present state, the default cryptographic library provides Ed25519 digital signatures and verification, using SHA-3.

### 2.2. Chaincode

### 2.3. Domains and assets



### 2.4. Transactions

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


### 2.5. Consensus

Iroha introduces a Byzantine Fault Tolerant consensus algorithm called Sumeragi. It is heavily inspired by the B-Chain algorithm:

Duan, S., Meling, H., Peisert, S., & Zhang, H. (2014). Bchain: Byzantine replication with high throughput and embedded reconfiguration. In International Conference on Principles of Distributed Systems (pp. 91-106). Springer.

Consensus in Sumeragi is performed on individual transactions and on the global state resulting from the application of the transaction. When a validating peer receives a transaction over the network, 


### 2.6. Data synchronization and retrieval

The state with the Merkle root that has 2f+1 signatures of validating servers is the most advanced state.

## Appendix

### A.1. Developing for Iroha

To ease development, Iroha makes use of the Python Fabric library (not to be confused with Hyperledger Fabric, the Python Fabric library enables running scripted commands on local and remote servers).