	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	sub 	x1,x1,x5
	sub 	x1,x1,x4
	sub 	x1,x1,x3
	sub 	x1,x1,x2
	sub 	x1,x1,x5
	sub 	x1,x1,x4
	sub 	x1,x1,x3
	sub 	x1,x1,x2
	subw  x6,x5,x4
	.word	0xddffccff
	.size	_start, .-_start
