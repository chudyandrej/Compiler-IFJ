PRJ=ifj
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS) 

$(PRJ): main.c tac.c tac.h err.c err.h garbage.c garbage.h ial.c ial.h parser.c parser.h precedence.c precedence.h scanner.c scanner.h stack.c stack.h str.c str.h buildin.c buildin.h
	$(CC) $(CFLAGS) -o $@ main.c tac.c tac.h err.c err.h garbage.c garbage.h ial.c ial.h parser.c parser.h precedence.c precedence.h scanner.c scanner.h stack.c stack.h str.c str.h  buildin.c buildin.h

clean:
	rm -f *.o *.out $(PROGS)
#
