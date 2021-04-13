	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	sll x3,x1,x2
	slli x4,x1,2
	slliw x5,x1,1
	.word	0xddffccff
	.size	_start, .-_start
