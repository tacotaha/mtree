CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LFLAGS=-lssl -lcrypto
EXEC=mtree test

all: $(EXEC)

mtree: main.o mtree.o
	$(CC) main.o mtree.o -o mtree  $(LFLAGS)

test: test.o mtree.o
	$(CC) test.o mtree.o -o test $(LFLAGS)

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c -o main.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

mtree.o: mtree.h mtree.c
	$(CC) $(CFLAGS) -c mtree.c -o mtree.o

clean:
	rm -f $(EXEC) *.o *~

format:
	find . -name "*.c" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
	find . -name "*.h" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut
