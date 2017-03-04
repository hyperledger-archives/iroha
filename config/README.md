# Note

Whenever you change `config.json`, don't forget to change according [`config.json`](../docker/build/config/config.json) in `docker` directory. 

`config.json` in this folder is used if you use [local build](https://github.com/hyperledger/iroha/blob/master/docs/how_to_build.rst) (without docker),
but `config.json` in `docker/build/config/config.json` is used inside docker images.

The reason behind this is that docker can not use files outside its build scope (which is the directory with Dockerfile).