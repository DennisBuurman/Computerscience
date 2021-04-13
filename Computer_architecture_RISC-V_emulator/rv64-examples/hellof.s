
hellof.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	000107b7          	lui	a5,0x10
   100b4:	14078793          	addi	a5,a5,320 # 10140 <_start+0x34>
   100b8:	06800713          	li	a4,104
   100bc:	20e00023          	sb	a4,512(zero) # 200 <main-0xfeb0>
   100c0:	00178793          	addi	a5,a5,1
   100c4:	0007c703          	lbu	a4,0(a5)
   100c8:	fe071ae3          	bnez	a4,100bc <main+0xc>
   100cc:	00a00793          	li	a5,10
   100d0:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   100d4:	00000513          	li	a0,0
   100d8:	00008067          	ret

00000000000100dc <putchar>:
   100dc:	0ff57513          	andi	a0,a0,255
   100e0:	20a00023          	sb	a0,512(zero) # 200 <main-0xfeb0>
   100e4:	00000513          	li	a0,0
   100e8:	00008067          	ret

00000000000100ec <puts>:
   100ec:	00c0006f          	j	100f8 <puts+0xc>
   100f0:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   100f4:	00150513          	addi	a0,a0,1
   100f8:	00054783          	lbu	a5,0(a0)
   100fc:	fe079ae3          	bnez	a5,100f0 <puts+0x4>
   10100:	00a00793          	li	a5,10
   10104:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   10108:	00008067          	ret

000000000001010c <_start>:
   1010c:	00001197          	auipc	gp,0x1
   10110:	6f418193          	addi	gp,gp,1780 # 11800 <__global_pointer$>
   10114:	00012137          	lui	sp,0x12
   10118:	00010113          	mv	sp,sp
   1011c:	ff010113          	addi	sp,sp,-16 # 11ff0 <__global_pointer$+0x7f0>
   10120:	f91ff0ef          	jal	ra,100b0 <main>
   10124:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe38>
   10128:	00000013          	nop
   1012c:	00000013          	nop
   10130:	00000013          	nop
   10134:	00000013          	nop
   10138:	00000013          	nop
   1013c:	00000013          	nop
