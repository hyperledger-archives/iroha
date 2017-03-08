# among tested images only with ubuntu works
FROM ubuntu:latest

COPY iroha /iroha

ENV IROHA_HOME=/iroha
ENV LD_LIBRARY_PATH=/iroha/libx86_64

WORKDIR /iroha

RUN useradd -ms /bin/bash iroha
USER iroha

CMD /iroha/bin/iroha-main
