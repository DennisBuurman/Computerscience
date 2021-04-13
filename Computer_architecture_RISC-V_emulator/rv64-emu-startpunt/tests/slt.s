	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	sltu  x4,x2,x1
	sltu  x5,x1,x2
	sltiu x6,x2,3
	sltiu x7,x2,8
	.word	0xddffccff
	.size	_start, .-_start
