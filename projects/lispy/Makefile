CFILES=$(wildcard *.c)
obj=$(CFILES:.c=.o)
COMPILER=gcc
COPTIONS=-std=c99 -Wall
OUTPUT=lispy
CLLIBS=-ledit -lm
DOPTIONS=-g

lispy: ${obj}
	$(COMPILER) $(COPTIONS) $(CFILES) $(CLLIBS) -o $(OUTPUT)

debug:
	$(COMPILER) $(COPTIONS) $(DOPTIONS) $(CFILES) $(CLLIBS) -o $(OUTPUT)

.PHONY: clean
clean:
	rm -f $(obj) lispy
