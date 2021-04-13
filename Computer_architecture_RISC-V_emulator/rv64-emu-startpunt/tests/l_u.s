	.data
	.align 8
	.local	A
A:
	.int 10, 20, 30, 40, 50, 60, 70, 80
	.size	A, .-A
	.text
	.align 4
	.globl	_start
	.type	_start, @function
_start:
	lhu x2,(x1)
	lbu x3,4(x1)
	lwu x4,8(x1)
	.word	0xddffccff
	.size	_start, .-_start
