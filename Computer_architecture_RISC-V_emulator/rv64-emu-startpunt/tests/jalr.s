	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	nop
	jalr x1,x2,8
	nop
	nop
	nop
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
