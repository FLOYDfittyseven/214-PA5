CC = gcc
CFLAGS = -Wall -Werror -g -pedantic -std=c99
COMPILE = $(CC) $(CFLAGS)

all: test1 test2 test3 test4 test5

test1: test1.o mem.o
	$(COMPILE) -o test1 test1.c mem.c

test2: test2.o mem.o
	$(COMPILE) -o test2 test2.c mem.c

test3: test3.o mem.o
	$(COMPILE) -o test3 test3.c mem.c

test4: test4.o mem.o
	$(COMPILE) -o test4 test4.c mem.c

test5: test5.o mem.o
	$(COMPILE) -o test5 test5.c mem.c

test1.o: test1.c
	$(COMPILE) -c test1.c

test2.o: test2.c
	$(COMPILE) -c test2.c

test3.o: test3.c
	$(COMPILE) -c test3.c

test4.o: test4.c
	$(COMPILE) -c test4.c

test5.o: test5.c
	$(COMPILE) -c test5.c

mem.o: mem.c mem.h
	$(COMPILE) -c mem.c

clean:
	rm -rf *.o test1 test2 test3 test4 test5
