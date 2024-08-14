COMPILER=gcc
COPTIONS=-std=c99 -Wall
OUTPUT=lispy
CLLIBS=-ledit -lm

all:
	$(COMPILER) $(COPTIONS) parsing.c mpc.c $(CLLIBS) -o $(OUTPUT)

clean:
	rm -f lispy
