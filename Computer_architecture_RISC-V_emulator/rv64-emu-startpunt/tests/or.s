	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	or   x3,x1,x2
	.word	0xddffccff
	.size	_start, .-_start
