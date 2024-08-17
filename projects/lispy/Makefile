CFILES=main.c parser.c lval.c lenv.c ltype.c builtin.c mpc.c
COMPILER=gcc
COPTIONS=-std=c99 -Wall 
OUTPUT=lispy
CLLIBS=-ledit -lm
DOPTIONS=-g

all:
	$(COMPILER) $(COPTIONS) $(CFILES) $(CLLIBS) -o $(OUTPUT)

debug:
	$(COMPILER) $(COPTIONS) $(DOPTIONS) $(CFILES) $(CLLIBS) -o $(OUTPUT)

clean:
	rm -f lispy
