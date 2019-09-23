CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: resolver

hw1: resolver.o
	${CC} resolver.o -o resolver

hw1.o: resolver.c
	${CC} ${CFLAGS} resolver.c

clean:
	rm -f *.o *~
