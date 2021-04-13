	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	lui x1,5
	lui x2,10
	lui x3,0
	lui x4,100
	.word	0xddffccff
	.size	_start, .-_start
