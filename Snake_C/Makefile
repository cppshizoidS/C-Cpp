CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -std=gnu11
LDFLAGS=-lrt

.PHONY: all clean

all: snake

snake: snake.c
	$(CC) $(CFLAGS) snake.c $(LDFLAGS) -o snake

clean:
	rm -f snake
