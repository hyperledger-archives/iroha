いろは構築手順 / Iroha build method
===================================

環境 / Environment
==================

今回は"sudo"を書きたくないのでrootで行いましたが普通のユーザーの方がいいと思います。よしなにお願いします

I could not write "sudo" in this time and I executed as root, but I
think normal user is better, Do as you think best.

OS
--

::

    root@mizuki # lsb_release -a
    No LSB modules are available.
    Distributor ID: Ubuntu
    Description:  Ubuntu 16.04.1 LTS
    Release:  16.04
    Codename: xenial

Compiler & Maketool
-------------------

基本的にC++14がコンパイルできる物が必要

I need C++14 compilable compiler

::

    root@mizuki # g++ -v
    gcc version 5.4.1 20160904 (Ubuntu 5.4.1-2ubuntu1~16.04)

::

    root@mizuki # cmake  -version
    cmake version 2.8.0

依存ライブラリのインストール / install dependencies
========================================================

::

    # apt -y install xsltproc libhdf5-serial-dev libsnappy-dev liblmdb-dev autoconf automake libtool unzip


protobuf のインストール / Install protobuf
------------------------------------------

buildは時間かかる あと\ **バージョンは3.0.0でなければいけない**

**Version should be 3.0.0!!**

::

    # cd /tmp; git clone -b v3.0.0 https://github.com/google/protobuf.git
    # cd /tmp/protobuf;(git cherry-pick 1760feb621a913189b90fe8595fffb74bce84598; echo Force continue)
    # cd /tmp/protobuf; ./autogen.sh; ./configure --prefix=/usr;
    # make -j 16;
    # make install
    # protoc --version

::

    root@mizuki # protoc --version
    libprotoc 3.0.0

grpc のインストール / Install grpc
----------------------------------

buildに時間かかるのでBinaryがほしい

::

    cd /tmp; git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc
    cd /tmp/grpc; git submodule update --init; make -j 14; make install

::

    root@mizuki # which grpc_cpp_plugin
    /usr/local/bin/grpc_cpp_plugin

Iroha本体のclone / Clone iroha
==============================

いろはを構築したい場所をよしなに決めてください

Could you decide place you will install iroha as you think best.

::

    ~ # git clone --recursive https://github.com/hyperledger/iroha.git
    ~ # ls -l
    total 4
    drwxr-xr-x 8 root root 4096 Dec  8 17:15 iroha

IROHA\_HOMEの設定 / set IROHA\_HOME
-----------------------------------

::

    ~ # cd iroha
    ~/iroha # export IROHA_HOME=$(pwd)
    ~/iroha # echo $IROHA_HOME
    /root/iroha


Iroha本体のbuild / build iroha
==============================

::

    ~/iroha # mkdir build
    ~/iroha # cd build/
    ~/iroha/build # cmake ..
    .
    .
    .
    [ 98%] Built target sumeragi_test
    [100%] Built target iroha-main
    root@mizuki ~/iroha/build#

完成！ Complate!
