CC 	= gcc
MFLAGS  = -I../include -I../../../shared/libosmocore/include
CFLAGS  = $(MFLAGS) -Wall -c
LDFLAGS = -o
OSMOPATH = /root/DP/libosmocore/src
LIBOSMOCORE = $(OSMOPATH)/msgb.o $(OSMOPATH)/panic.o $(OSMOPATH)/talloc.o $(OSMOPATH)/backtrace.o  $(OSMOPATH)/logging.o $(OSMOPATH)/utils.o

OBJ = layer1.o L2toL1int.o L1toL2int.o btsToMobile.o $(LIBOSMOCORE)
BTS_OBJ = BTS.o $(LIBOSMOCORE)

.PHONY: all clean

all: layer1 bts

bts: $(BTS_OBJ)
	$(CC) $(BTS_OBJ) -o BTS

BTS.o: BTS.c
	$(CC) $(CFLAGS) -o BTS.o BTS.c

layer1: $(OBJ)
	$(CC) $(OBJ) -o layer1

layer1.o: layer1.c
	$(CC) $(CFLAGS) -o layer1.o layer1.c

L1toL2int.o: L1toL2int.c
	$(CC) $(CFLAGS) -o L1toL2int.o L1toL2int.c

L2toL1int.o: L2toL1int.c 
	$(CC) $(CFLAGS) -o L2toL1int.o L2toL1int.c

btsToMobile.o: btsToMobile.c 
	$(CC) $(CFLAGS) -o btsToMobile.o btsToMobile.c

clean:
	rm -f *.o
	rm -f layer1
