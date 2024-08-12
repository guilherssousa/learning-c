CC=gcc

all: main

main: main.c
	$(CC) `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`

clean:
	rm -f a.out
