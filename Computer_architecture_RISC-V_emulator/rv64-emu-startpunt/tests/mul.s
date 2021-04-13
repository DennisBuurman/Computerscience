	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	mul x3,x2,x1
	.word	0xddffccff
	.size	_start, .-_start
