FROM ubuntu:16.04

RUN apt-get update; \
    apt-get install -y libc-ares-dev

#Install iroha
COPY iroha.deb /tmp/iroha.deb
RUN apt-get install -y /tmp/iroha.deb; \
    rm -f /tmp/iroha.deb

WORKDIR /opt/iroha_data

COPY entrypoint.sh wait-for-it.sh /
RUN chmod +x /entrypoint.sh /wait-for-it.sh; \
    groupadd -r iroha && useradd --no-log-init -r -g iroha iroha

USER iroha
ENTRYPOINT ["/entrypoint.sh"]
CMD ["/sbin/init"]
