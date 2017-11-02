FROM ubuntu:16.04

ENV IROHA_HOME /opt/iroha
ENV IROHA_BUILD /opt/iroha/build

COPY iroha.deb /tmp/iroha.deb
RUN apt-get update;\
    apt-get upgrade; \
    apt-get install -y \
        libc-ares-dev \
        gdebi-core

#Install iroha
RUN gdebi /tmp/iroha.deb

CMD ["bash"]
