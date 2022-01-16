CC=gcc
CFLAGS=-O2

datalink: datalink.o protocol.o lprintf.o crc32.o
	gcc datalink.o protocol.o lprintf.o crc32.o -o datalink -lm

clean:
	${RM} *.o datalink *.log

