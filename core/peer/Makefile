all: peer

CC=g++
CFLAGS = -O2 -Wall -Wextra -std=c++14
INCLUDES = -I../lib -I../vendor/Aeron/aeron-client/src/main/cpp -I../vendor/leveldb/include
LIBRARYS = -L../vendor/leveldb/out-static -lsnappy -lleveldb -lssl -lcrypto -L/usr/lib -lyaml-cpp -lpthread -L/usr/lib ../vendor/Aeron/cppbuild/Debug/lib/libaeron_client.a

../db/ConsensusRepository.o:
	make -C ../db

../util/exception.o:
	make -C ../util
OBJECTS := $(OBJECTS) ../util/exception.o

pbft-sieve.o:
	make -C ../consensus
OBJECTS := ../consensus/pbft-sieve.o $(OBJECTS)

../crypto/hash.o:
	make -C ../crypto
OBJECTS := $(OBJECTS) ../crypto/hash.o ../crypto/bin/*

connection.o: connection.cpp
	echo "connectionをBuildします"
	$(CC) $(CFLAGS) $+ -o $@ $(INCLUDES) $(LIBRARYS) -c
OBJECTS := connection.o $(OBJECTS)

peer: peer.cpp $(OBJECTS)
	echo "peerをBuildします"
	$(CC) $(CFLAGS) $+ -o $@ $(INCLUDES) $(LIBRARYS)

.PHONY: clean
clean:
	- $(RM) $(OBJECTS) $(TARGET) peer
	make -C ../crypto clean
	make -C ../util clean
	make -C ../db clean
