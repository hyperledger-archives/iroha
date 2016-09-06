all: test.o exception.o

LIBPATH = ../vendor/KeccakCodePackage
LIBRARYS = -lssl -lcrypto -L/usr/lib
MAKE = gmake
CC=g++
CFLAGS := $(CFLAGS) -std=c++14

test.o:
	$(CC) $(CFLAGS) test.cpp -o $@ -c

exception.o:
	$(CC) $(CFLAGS) exception.cpp -o $@ -c


.PHONY: clean
clean:
	- rm -rf test.o
