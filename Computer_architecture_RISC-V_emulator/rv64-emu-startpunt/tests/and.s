	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	and  x3,x1,x2
	andi x4,x1,16
	.word	0xddffccff
	.size	_start, .-_start
