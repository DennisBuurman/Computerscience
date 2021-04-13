	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	xori  x2,x1,15
	xor   x3,x1,x2
	.word	0xddffccff
	.size	_start, .-_start
