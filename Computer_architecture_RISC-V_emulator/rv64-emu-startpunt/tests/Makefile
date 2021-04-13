#
# rv64-emu -- Simple 64-bit RISC-V simulator
#
# Copyright (C) 2017  Leiden University, The Netherlands.
#

SHELL = /bin/bash

runtests:	../rv64-emu
		@echo "Running unit tests ..."
		@pass=0;failed=0;			\
		for testfile in *.conf; do	\
			echo -en "+ $$testfile\t";	\
			../rv64-emu -t $$testfile;	\
			if [ $$? -eq 0 ]; then		\
				echo "OK";		\
				pass=$$((pass+1));	\
			else				\
				echo "FAIL";		\
				failed=$$((failed+1));	\
			fi;				\
		done;					\
		echo "$$pass passed; $$failed failed"

# Some notes on the compile command:
# - We explicitly specify the RISC-V architecture without 'c' extension
#   to suppress the generation of compressed instructions.
# - We explicitly specify addresses for the text and data segments: some
#   of our tests rely on it and the RISC-V toolchain seems to sometimes
#   change these.
%.bin:		%.s
		riscv64-unknown-elf-gcc -Ttext=0x10000 -Tdata=0x11100 \
			-Wall -march=rv64imafd -O0 \
			-nostdlib -fno-builtin -nodefaultlibs -o $@ $<
