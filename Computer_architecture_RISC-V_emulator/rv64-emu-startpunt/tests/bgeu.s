	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	bgeu x1,x2,4
	nop
	nop
	bgeu x2,x3,4
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
