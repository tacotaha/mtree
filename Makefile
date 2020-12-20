CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LFLAGS=-lssl -lcrypto
EXEC=mtree
OBJ=main.o mtree.o

mtree: main.o mtree.o
	$(CC) $(OBJ) -o mtree  $(LFLAGS)

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c -o main.o

mtree.o: mtree.h mtree.c
	$(CC) $(CFLAGS) -c mtree.c -o mtree.o

clean:
	rm -f $(EXEC) *.o *~
