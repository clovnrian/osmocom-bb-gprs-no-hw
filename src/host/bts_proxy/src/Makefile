CC 	= gcc
MFLAGS  = -I../include -I../../../../../libosmocore/include
CFLAGS  = $(MFLAGS) -Wall -c -g3
LDFLAGS = -o
OSMOPATH = ../../../../../libosmocore/src
LIBOSMOCORE = $(OSMOPATH)/msgb.o $(OSMOPATH)/panic.o $(OSMOPATH)/talloc.o $(OSMOPATH)/backtrace.o  $(OSMOPATH)/logging.o $(OSMOPATH)/utils.o

OBJ = proxy.o L2toL1int.o L1toL2int.o btsToMobile.o $(LIBOSMOCORE)
BTS_OBJ = BTS.o $(LIBOSMOCORE)

.PHONY: all

all: proxy bts

bts: $(BTS_OBJ)
	$(CC) -g -O0 $(BTS_OBJ) -o BTS/BTS

BTS.o: BTS/main.c
	$(CC) $(CFLAGS) -o BTS.o BTS/main.c

proxy: $(OBJ)
	$(CC) -g -O0 $(OBJ) -o proxy/proxy

proxy.o: proxy/main.c
	$(CC) $(CFLAGS) -o proxy.o proxy/main.c

L1toL2int.o: proxy/L1toL2int.c
	$(CC) $(CFLAGS) -o L1toL2int.o proxy/L1toL2int.c

L2toL1int.o: proxy/L2toL1int.c 
	$(CC) $(CFLAGS) -o L2toL1int.o proxy/L2toL1int.c

btsToMobile.o: proxy/btsToMobile.c 
	$(CC) $(CFLAGS) -o btsToMobile.o proxy/btsToMobile.c

.PHONY: clean

clean:
	rm -f proxy/*.o
	rm -f proxy/proxy
	rm -f BTS/*.o
	rm -f BTS/BTS
