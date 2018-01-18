Deploying Iroha
===============

Hyperledger Iroha can be deployed in different ways, depending on the perspective and the purpose. There can be either a single node deployed, or multiple nodes running in several containers on local machine or spreaded across the network — so pick any case you need. This page describes different scenarios and is intended to act as how-to guide for users, primarily trying out Iroha for the first time.

Running single instance  
^^^^^^^^^^^^^^^^^^^^^^^

Generally, people want to run Iroha locally in order to try out the API and explore the capabilites. This can be done in local or container environment (Docker). We will expore both possible cases, but in order to simplify peer components deployment, *it is advised to have Docker installed on your machine*.

Local enviroment
----------------

By local enviroment, it is meant to have daemon process and the components, which it relies onto (Redis and Postgres) deployed without any containers. This might be helpful in cases when messing up with Docker is not preferred — generally a quick exploration of the features.

Run postgres server
"""""""""""""""""""

In order to run postgres server locally, you should check postgres `website <https://www.postgresql.org/docs/current/static/server-start.html>`__ and follow their description. Generally, postgres server runs automatically when system starts, but this should be checked in the configuration of the system. 

Run redis server
""""""""""""""""

Redis is known for simple run process. When it is installed in the system, it enough to execute `redis-server` binary without arguments. Just in case, check their `website <https://redis.io/topics/quickstart>`__.

Run iroha daemon (irohad)
"""""""""""""""""""""""""

There is a list of assumptions which you should review before proceeding:

 * Postgres server is up and running, the port is 5432
 * Redis server is up and running, the port is 6379
 * Iroha daemon binary is built and accessible in your system
 * The genesis block and configuration files were created
 * Config file uses valid postgres and redis connection settings
 * A keypair for the peer is generated
 * This is the first time you run the Iroha on this peer and you want to create new chain

.. Hint:: Have you got something that is not the same as in the list of assumptions? Please, refer to the section below the document, titled as "Dealing with troubles".

In case of valid assumptions, the only thing that remains is to launch the daemon process with following parameters:

+---------------+-----------------------------------------------------------------+
| Parameter     | Meaning                                                         |
+---------------+-----------------------------------------------------------------+
| config        | configuration file, containing postgres, and redis connection,  |
|               | and values to tune the system                                   |
+---------------+-----------------------------------------------------------------+
| genesis_block | initial block in the ledger                                     |
+---------------+-----------------------------------------------------------------+
| keypair_name  | private and public key file names without file extension,       |
|               | used by peer to sign the blocks                                 |
+---------------+-----------------------------------------------------------------+

An example of shell command, running iroha dameon is 

.. code-block:: shell

    irohad --config example/config.sample --genesis_block example/genesis.block --keypair_name example/node0

.. Attention:: If you have stopped the daemon and want to use existing chain — you should not pass the genesis block parameter.


Docker
------

In order to run Iroha peer as a single instance in Docker, you should pull the image for iroha first:

.. code-block:: shell

    docker pull hyperledger/iroha-docker:latest

.. Hint:: Use *latest* tag for latest stable release, and *develop* for latest development version  

Then, you have to create an enviroment for the image to run without problems:

Create docker network
"""""""""""""""""""""

Containers for Redis, Postgres, and Iroha should run in the same virtual network, in order to be available to each other. Create a network, by typing following command (you can use any name for the network, but in the example we use *iroha-network* name): 

.. code-block:: shell

    docker network create iroha-network
    
Run Redis in a container
""""""""""""""""""""""""

Run redis server, attaching it to the network you have created before, and exposing ports for communication:

.. code-block:: shell

    docker run --name some-redis \
    -p 6379:6379 \
    --network=iroha-network \
    -d redis:3.2.8

Run Postgresql in a container
"""""""""""""""""""""""""""""

Similarly, run postgres server, attaching it to the network you have created before, and exposing ports for communication:

.. code-block:: shell

    docker run --name some-postgres \
    -e POSTGRES_USER=postgres \
    -e POSTGRES_PASSWORD=mysecretpassword \
    -p 5432:5432 \
    --network=iroha-network \
    -d postgres:9.5

Create volume for block storage
"""""""""""""""""""""""""""""""

Before we run iroha daemon in the container, we should create persistent volume to store files, storing blocks for the chain. It is done via following command:

.. code-block:: shell

    docker volume create blockstore

Running iroha daemon in docker container
""""""""""""""""""""""""""""""""""""""""

There is a list of assumptions which you should review before proceeding:
 * Postgres and redis servers are running in the same docker network
 * There is a folder, containing config file and keypair for a single node
 * This is the first time you run the Iroha on this peer and you want to create new chain

If they are met, you can move forward with the following command:

.. code-block:: shell

    docker run --name iroha \
    # External port
    -p 50051:50051 \
    # Folder with configuration files
    -v ~/Developer/iroha/example:/opt/iroha_data \
    # Blockstore volume
    -v blockstore:/tmp/block_store \
    # Postgres settings
    -e POSTGRES_HOST='some-postgres' \
    -e POSTGRES_PORT='5432' \
    -e POSTGRES_PASSWORD='mysecretpassword' \
    -e POSTGRES_USER='postgres' \
    # Redis settings
    -e REDIS_HOST='some-redis' \
    -e REDIS_PORT='6379' \
    # Node keypair name
    -e KEY='node0' \
    # Docker network name
    --network=iroha-network \
    hyperledger/iroha-docker:latest

Dealing with troubles
^^^^^^^^^^^^^^^^^^^^^

—"Please, help me, because I am…"

Not having Iroha daemon binary
------------------------------

Not having config file
----------------------

Not having genesis block
------------------------

Not having a keypair for a peer
-------------------------------


