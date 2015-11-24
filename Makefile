PRJ=ial
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c tac.c stack.h tmp.h
	$(CC) $(CFLAGS) -o $@ $(PRJ).c tac.c stack.c tmp.c $(PRJ)-test.c

clean:
	rm -f *.o *.out $(PROGS)
#
