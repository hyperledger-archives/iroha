FROM ubuntu:16.04

ENV IROHA_HOME /opt/iroha
ENV IROHA_BUILD /opt/iroha/build
ARG node

COPY iroha.deb /tmp/iroha.deb
COPY entrypoint.sh /entrypoint.sh
RUN apt-get update;\
    apt-get upgrade; \
    apt-get install -y \
        libc-ares-dev \
        gdebi-core

#Install iroha
RUN gdebi --non-interactive /tmp/iroha.deb

WORKDIR /op/iroha

ENTRYPOINT ["/entrypoint.sh"]
