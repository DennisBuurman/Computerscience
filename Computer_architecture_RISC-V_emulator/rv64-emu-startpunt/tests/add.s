	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	add 	x1,x1,x2
	add 	x1,x1,x3
	add 	x1,x1,x4
	add 	x1,x1,x5
	add 	x1,x1,x2
	add 	x1,x1,x3
	add 	x1,x1,x4
	add 	x1,x1,x5
	addi  x6,x5,1
	addiw x7,x6,3
	addw  x8,x7,x7
	.word	0xddffccff
	.size	_start, .-_start
