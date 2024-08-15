COMPILER=gcc
COPTIONS=-std=c99 -Wall
OUTPUT=lispy
CLLIBS=-ledit -lm
DOPTIONS=-g

all:
	$(COMPILER) $(COPTIONS) parsing.c mpc.c $(CLLIBS) -o $(OUTPUT)

debug:
	$(COMPILER) $(COPTIONS) $(DOPTIONS) parsing.c mpc.c $(CLLIBS) -o $(OUTPUT)

clean:
	rm -f lispy
