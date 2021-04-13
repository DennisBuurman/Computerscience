	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	srl    x3,x1,x2
	srliw  x4,x1,1
	srli   x5,x1,4
	sraiw  x6,x1,3
	.word	0xddffccff
	.size	_start, .-_start
