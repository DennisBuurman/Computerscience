	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	add	x1,x1,x2		
	auipc x1,4
	nop
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
