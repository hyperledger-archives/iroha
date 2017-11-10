FROM ubuntu:16.04

ENV IROHA_HOME /opt/iroha
ENV IROHA_BUILD /opt/iroha/build
ARG node

RUN apt-get update;\
    apt-get upgrade; \
    apt-get install -y \
        libc-ares-dev \
        gdebi-core

#Install iroha
COPY iroha.deb /tmp/iroha.deb
RUN gdebi --non-interactive /tmp/iroha.deb

WORKDIR /opt/iroha

COPY entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
