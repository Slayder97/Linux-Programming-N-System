.PHONY : all clean

CFLAGS := -Wall
CC := gcc

all:
	$(CC) -o client client.c $(CFLAGS)
	$(CC) -o server server.c $(CFLAGS)

clean:
	rm -rf  client server