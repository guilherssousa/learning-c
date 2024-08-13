CC=gcc
HEADER=./include/
OUTPUT_DIR=./out

all: main

main: main.c
	$(CC) -I$(HEADER) -o $(OUTPUT_DIR)/gsh main.c builtin.c

clean:
	rm -rf $(OUTPUT_DIR)
