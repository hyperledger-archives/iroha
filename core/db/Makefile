all: repository.o ConsensusRepository.o

CC=g++
CFLAGS = -O2 -Wall -Wextra -std=c++14
INCLUDES = -I../vendor/leveldb/include
LIBRARYS = -L../vendor/leveldb/out-static -lsnappy -lleveldb

repository.o:
	$(CC) $(CFLAGS) $+ repository.hpp $(INCLUDES) $(LIBRARYS) -c

ConsensusRepository.o:
	$(CC) $(CFLAGS) $+ ConsensusRepository.hpp $(INCLUDES) $(LIBRARYS) -c

test: test.cpp
	$(CC) $(CFLAGS) $+  repository.hpp $(INCLUDES) $(LIBRARYS)

.PHONY: clean
clean:
	- $(RM) $(OBJECTS) test *.o
