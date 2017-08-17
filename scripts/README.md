# How to build `iroha` using docker

# Git repository

Clone `iroha` repository on your directory.

```bash
git clone https://github.com/hyperledger/iroha /path/to/iroha
```

# Development environment

Build on docker container for develop environment `hyperledger/iroha-docker-develop` so you need to build it:

```bash
docker build -t hyperledger/iroha-docker-develop /path/to/iroha/docker/develop
```

# How to run

```bash
/path/to/iroha/scripts/run-iroha.dev.sh
```

You will be attached to interactive environment for development and testing

# Troubleshooting

To successfully build docker image, you should be able to build it locally: [follow this guilde](../docs/how_to_build.rst) in case of troubles.
