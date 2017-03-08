# Description

In order to improve build speed of iroha and make it more stable and testable, `tiny` docker image is created.

It uses pre-built shared objects for linux x86_64 (which circle-ci is using) and very small Dockerfile.

It will be used mainly to ease build and test of iroha.

Also (probably) it will be used as minified production version.


# Note

There are binary libraries in lib* directory. We use them to copy inside tiny container.
Do not remove them please.

# How to build `iroha` using docker

Clone `iroha.git` on your directory.

```
git clone https://github.com/hyperledger/iroha.git
cd iroha
```

Build depends on the environment variable `IROHA_HOME` so you need to set it:

`export IROHA_HOME=$(pwd)`

Run build script and wait for completion. 
```
${IROHA_HOME}/docker/build.sh
``` 


# Usage example

In order to use iroha, you need to create `${IROH_HOME}/config/sumeragi.json` config file in each node (docker container). 
Create **valid** `sumeragi.json` for each node and run:

```
docker run -v /path/to/sumeragi.json:/iroha/config/sumeragin.json hyperledger/iroha-docker
``` 


#### Good luck!
