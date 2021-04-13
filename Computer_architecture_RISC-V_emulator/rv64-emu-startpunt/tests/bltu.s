	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	bltu x1,x2,4
	nop
	nop
	bltu x2,x3,4
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
