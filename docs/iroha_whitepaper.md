---

## 1. Overview

### 1.1. Relationship to Fabric and Sawtooth Lake

## 2. System architecture


### 2.1. Cryptography

Iroha aims to provide modular cryptographic functionality. At the present state, the default cryptographic library provides Ed25519 digital signatures and verification, using SHA-3.

$ x = \dfrac{-b \pm \sqrt{b^2 - 4ac}}{2a} \sum $

### 2.2. Transactions

Transaction types are as follows:

* Domain registration
* Transfer
* Message blob


### 2.3. Consensus

Iroha introduces a Byzantine Fault Tolerant consensus algorithm called Sumeragi.

B-Chain:

Duan, S., Meling, H., Peisert, S., & Zhang, H. (2014). Bchain: Byzantine replication with high throughput and embedded reconfiguration. In International Conference on Principles of Distributed Systems (pp. 91-106). Springer.


### 2.4. Data synchronization and retrieval