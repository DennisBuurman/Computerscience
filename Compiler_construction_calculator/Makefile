CC		= gcc
LEX		= flex
YACC		= bison -y
SHELL		= /bin/sh

WARNINGS	= -Wall
OTHERS		= -g -std=c++11
DEFINES		= -DDEBUG -D_POSIX_SOURCE=1

CFLAGS		= $(WARNINGS) $(OTHERS) $(DEFINES)
LFLAGS		=
YFLAGS		= --defines --debug --verbose
LDFLAGS		= -g
LOADLIBS	= -lfl -lm

IMPORTANT	= calc.[hly] \
		  Makefile

all: calc

# To be executed before the very first build
first: dirs

# Dependency stuff
calc: gen/y.tab.o gen/lex.yy.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBS)

gen/y.tab.h gen/y.tab.c: calc.y 
	$(YACC) $(YFLAGS) $< -o gen/y.tab.c

gen/lex.yy.c: calc.l 
	$(LEX) $(LFLAGS) -o$@ $<

clean:	
	rm -f gen/{lex.yy.c,y.tab.*,y.output,*.o} calc \
	    `find . -name core -o -name \*\.bak`

backup:
	tar cfz ../CoCo-`date +'%y%m%d-%H%M'`.tar.gz $(IMPORTANT)

dirs:
	mkdir gen

# Show dependencies between .c files
showdeps:
	$(CC) -MM *.c

# Dependency stuff
gen/lex.yy.o: gen/lex.yy.c gen/y.tab.h
gen/y.tab.o: gen/y.tab.c
