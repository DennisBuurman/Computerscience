# This is a simple Makefile for small projects.  When you  
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=gcc
CFLAGS=-lglut -lGLU -lGL -lm -lpthread
LIB=./SOUND/linux-gcc-64/libIrrKlang.so /lib/x86_64-linux-gnu/libpthread.so.0

main: main.cc
	$(CC) -o final main.cc -I"./SOUND/include/" -L"./SOUND/linux-gcc-64/" $(LIB) $(CFLAGS) 
