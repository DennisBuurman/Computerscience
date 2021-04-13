#
# Important: we did not include fpdemo.bin and mandel.bin here on purpose,
# since the compiler with soft-float support is not installed within the
# ca2018 environment. Please use the .bin files that are enclosed in the
# examples tarball.
#

TARGETS = basic.bin \
	hello.bin hellof.bin \
	comp.bin \
	matvec.bin matvecu.bin \
	brainfuck.bin

CC = riscv64-unknown-elf-gcc
CFLAGS = -march=rv64imafd -Wall -nostdlib -fno-builtin -nodefaultlibs -O2
CRT = minicrt-riscv.s

all:		$(TARGETS)

# hello.c, hellof.c and brainfuck.c do not require roman.c
hello.bin:	hello.c
		$(CC) $(CFLAGS) -o $@ $< $(CRT)

hellof.bin:	hellof.c
		$(CC) $(CFLAGS) -o $@ $< $(CRT)

brainfuck.bin:	brainfuck.c
		$(CC) $(CFLAGS) -O0 -o $@ $< $(CRT)


# By default, roman.c is included
%.bin:		%.c
		$(CC) $(CFLAGS) -o $@ $< roman.c $(CRT)

%.bin:		%.s
		$(CC) $(CFLAGS) -o $@ $<

clean:
		rm -f $(TARGETS)
