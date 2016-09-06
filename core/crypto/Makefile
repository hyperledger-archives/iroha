all: hash.o base64.o signature.o

LIBPATH = ../vendor/KeccakCodePackage
LIBRARYS = -lssl -lcrypto -L/usr/lib -L../vendor/ed25519 -led25519

BINDIR  = bin
DEFINE  = -DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded
INCLUDES := -I../vendor/ed25519/src -I../vendor/apple

$(BINDIR):
	mkdir -p $(BINDIR)

MAKE = gmake
CC=g++

CFLAGS := $(CFLAGS) -fomit-frame-pointer
CFLAGS := $(CFLAGS) -O2
CFLAGS := $(CFLAGS) -g0
CFLAGS := $(CFLAGS) -march=native
CFLAGS := $(CFLAGS) -mtune=native
CFLAGS := $(CFLAGS) -m64
CFLAGS := $(CFLAGS) -std=c++14


CFLAGS := $(CFLAGS) $(DEFINE)

SOURCES := $(SOURCES) $(LIBPATH)/KeccakCodePackage.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/

SOURCES := $(SOURCES) $(LIBPATH)/Common/align.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Common/

SOURCES := $(SOURCES) $(LIBPATH)/Common/brg_endian.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Common/

SOURCES := $(SOURCES) $(LIBPATH)/Common/brg_endian.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Common/

SOURCES := $(SOURCES) $(LIBPATH)/SnP/KeccakP-1600/Optimized/KeccakP-1600-64.macros
CFLAGS := $(CFLAGS) -I$(LIBPATH)/SnP/KeccakP-1600/Optimized/

SOURCES := $(SOURCES) $(LIBPATH)/SnP/KeccakP-1600/Optimized64/KeccakP-1600-SnP.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/SnP/KeccakP-1600/Optimized64/

SOURCES := $(SOURCES) $(LIBPATH)/SnP/SnP-Relaned.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/SnP/

SOURCES := $(SOURCES) $(LIBPATH)/SnP/KeccakP-1600/Optimized64/ufull/KeccakP-1600-opt64-config.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/SnP/KeccakP-1600/Optimized64/ufull/

SOURCES := $(SOURCES) $(LIBPATH)/Constructions/KeccakSponge.inc
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Constructions/

SOURCES := $(SOURCES) $(LIBPATH)/Constructions/KeccakSponge.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Constructions/

SOURCES := $(SOURCES) $(LIBPATH)/Modes/SimpleFIPS202.h
CFLAGS := $(CFLAGS) -I$(LIBPATH)/Modes/

SOURCES := $(SOURCES) $(LIBPATH)/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
$(BINDIR)/KeccakP-1600-opt64.o: $(LIBPATH)/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c $(HEADERS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakP-1600-opt64.o

SOURCES := $(SOURCES) $(LIBPATH)/Constructions/KeccakSponge.c
$(BINDIR)/KeccakSponge.o: $(LIBPATH)/Constructions/KeccakSponge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakSponge.o

SOURCES := $(SOURCES) $(LIBPATH)/Modes/SimpleFIPS202.c
$(BINDIR)/SimpleFIPS202.o: $(LIBPATH)/Modes/SimpleFIPS202.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/SimpleFIPS202.o

../vendor/ed25519/libed25519.a:
	make -C ../vendor ed25519

../util/test.o:
	make -C ../util
OBJECTS := $(OBJECTS) ../util/test.o

../util/exception.o:
	make -C ../util
OBJECTS := $(OBJECTS) ../util/exception.o

../vendor/apple/apple_base64.o:
	make -C ../vendor apple_base64
OBJECTS := $(OBJECTS) ../vendor/apple/apple_base64.o

hash.o: $(BINDIR) $(OBJECTS)
	$(CC) $(CFLAGS) hash.cpp $(LIBRARYS) -o $@ -c
OBJECTS := $(OBJECTS) hash.o

base64.o: $(BINDIR) $(OBJECTS)
	$(CC) $(CFLAGS) base64.cpp $(LIBRARYS) -o $@ -c
OBJECTS := $(OBJECTS) base64.o

signature.o: $(BINDIR) $(OBJECTS) ../vendor/ed25519/libed25519.a
	$(CC) $(CFLAGS) signature.cpp $(LIBRARYS) $(INCLUDES) -o $@ -c
OBJECTS := $(OBJECTS) signature.o

test: base64.o hash.o signature.o
	$(CC) $(CFLAGS) unit-test.cpp $(OBJECTS) $(LIBRARYS) -o $@
	- ./test

.PHONY: clean
clean:
	- rm -rf *.o ../util/*.o bin test
