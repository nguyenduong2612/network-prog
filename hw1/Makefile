CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: hw1

hw1: hw1.o
	${CC} hw1.o -o hw1

hw1.o: hw1.c
	${CC} ${CFLAGS} hw1.c

clean:
	rm -f *.o *~
