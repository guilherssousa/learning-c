COMPILER=gcc
COPTIONS=-std=c99 -Wall 
OUTPUT=lispy
CLLIBS=-ledit -lm
DOPTIONS=-g

all:
	$(COMPILER) $(COPTIONS) main.c parser.c lval.c lenv.c ltype.c builtin.c mpc.c $(CLLIBS) -o $(OUTPUT)

debug:
	$(COMPILER) $(COPTIONS) $(DOPTIONS) main.c parser.c lval.c lenv.c ltype.c builtin.c mpc.c $(CLLIBS) -o $(OUTPUT)

clean:
	rm -f lispy
