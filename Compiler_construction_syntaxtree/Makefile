#
# Makefile for assignment 2 of the Compiler Construction course.
# LIACS, Leiden University.
#
CC        = gcc -fPIC
CXX       = g++ -fPIC
LEX       = flex
YACC      = bison -y

WARNINGS  = -Wall
OTHERS    = -g --std=c++11
DEFINES   = -DDEBUG
IDIRS     = -I.

CFLAGS    = $(WARNINGS) $(OTHERS) $(DEFINES) $(IDIRS)
LFLAGS    = 
YFLAGS    = --defines --debug --verbose -Wno-yacc
LDFLAGS   = -g -std=c++11
LOADLIBS  = -lfl -lm
OBJDIR    = gen/
IMPORTANT	= comp.[hly] *.h *.c *.cc \
		  Makefile

.PHONY: all first clean dirs

all: Makefile comp

# To be executed before the very first build
first: dirs

Makefile: Makefile.in
	./config.status

# Dependency stuff
comp: $(OBJDIR)y.tab.o $(OBJDIR)lex.yy.o gen/debug.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LOADLIBS) -Lstatic -lcoco2

$(OBJDIR)y.tab.h $(OBJDIR)y.tab.c: comp.y 
	$(YACC) $(YFLAGS) $< -o $(OBJDIR)y.tab.c

$(OBJDIR)lex.yy.c: comp.l 
	$(LEX) $(LFLAGS) -o$@ $<

clean:	
	rm -f $(OBJDIR)* comp

dirs:
	mkdir gen

# Dependency stuff
$(OBJDIR)lex.yy.o: $(OBJDIR)lex.yy.c $(OBJDIR)y.tab.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)y.tab.o: $(OBJDIR)y.tab.c
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)%.o: %.c
	$(CXX) $(CFLAGS) -c -o $@ $<
