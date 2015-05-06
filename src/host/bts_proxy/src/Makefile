CC 	= gcc
INCLUDE  = -I ../include -I /usr/local/include
CFLAGS  = $(INCLUDE) -Wall -c -g3
LDFLAGS = -o
OSMOPATH = 
LIBOSMOCORE = -losmocore

OBJ = proxy.o L2toL1int.o L1toL2int.o btsToMobile.o L1ToBts.o
BTS_OBJ = BTS.o

.PHONY: all

all: proxy bts

bts: $(BTS_OBJ)
	$(CC) $(INCLUDE) $(BTS_OBJ) -g -O0 -o BTS/BTS $(LIBOSMOCORE)

BTS.o: BTS/main.c
	$(CC) $(CFLAGS) -o BTS.o BTS/main.c

proxy: $(OBJ)
	$(CC) $(INCLUDE) -g -O0 $(OBJ) -o proxy/proxy $(LIBOSMOCORE)

proxy.o: proxy/main.c
	$(CC) $(CFLAGS) -o proxy.o proxy/main.c

L1toL2int.o: proxy/L1toL2int.c
	$(CC) $(CFLAGS) -o L1toL2int.o proxy/L1toL2int.c

L2toL1int.o: proxy/L2toL1int.c 
	$(CC) $(CFLAGS) -o L2toL1int.o proxy/L2toL1int.c

btsToMobile.o: proxy/btsToMobile.c 
	$(CC) $(CFLAGS) -o btsToMobile.o proxy/btsToMobile.c
	
L1ToBts.o: proxy/L1ToBts.c 
	$(CC) $(CFLAGS) -o L1ToBts.o proxy/L1ToBts.c

.PHONY: clean

clean:
	rm -f proxy/*.o
	rm -f proxy/proxy
	rm -f BTS/*.o
	rm -f BTS/BTS
	rm -f *.o
