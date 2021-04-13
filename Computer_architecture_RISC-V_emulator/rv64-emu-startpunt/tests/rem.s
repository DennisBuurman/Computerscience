	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	remw x3,x1,x2
	remw x4,x2,x1
	.word	0xddffccff
	.size	_start, .-_start
