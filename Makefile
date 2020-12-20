CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LFLAGS=-lssl -lcrypto
EXEC=main

all: $(EXEC)

main: main.c 
	$(CC) $(CFLAGS) main.c  $(LFLAGS) -o main

clean:
	rm -f $(EXEC) *.o *~
