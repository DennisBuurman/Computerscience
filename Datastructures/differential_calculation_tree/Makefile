CC=g++
CFLAGS=-c -Wall -std=c++11 -Wextra -pedantic
LDFLAGS=
SOURCES=main.cc tree.cc node.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=Expressies

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf *o $(EXECUTABLE)
