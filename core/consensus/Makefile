all: pbft-sieve.o

CC=g++
CFLAGS = -O2 -Wall -Wextra -std=c++14
INCLUDES = -I../vendor/leveldb/include
LIBRARYS = -L../vendor/leveldb/out-static -lsnappy -lleveldb

pbft-sieve.o: pbft-sieve.cpp
	$(CC) $(CFLAGS) $+ pbft-sieve.cpp $(INCLUDES) $(LIBRARYS) -c

.PHONY: clean
clean:
	- $(RM) $(OBJECTS) pbft-sieve.o
