
hello.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	20000793          	li	a5,512
   100b4:	06800713          	li	a4,104
   100b8:	00e78023          	sb	a4,0(a5)
   100bc:	06500713          	li	a4,101
   100c0:	00e78023          	sb	a4,0(a5)
   100c4:	06c00713          	li	a4,108
   100c8:	00e78023          	sb	a4,0(a5)
   100cc:	00e78023          	sb	a4,0(a5)
   100d0:	06f00713          	li	a4,111
   100d4:	00e78023          	sb	a4,0(a5)
   100d8:	00a00713          	li	a4,10
   100dc:	00e78023          	sb	a4,0(a5)
   100e0:	00000513          	li	a0,0
   100e4:	00008067          	ret

00000000000100e8 <_start>:
   100e8:	00002197          	auipc	gp,0x2
   100ec:	83418193          	addi	gp,gp,-1996 # 1191c <__global_pointer$>
   100f0:	00012137          	lui	sp,0x12
   100f4:	12010113          	addi	sp,sp,288 # 12120 <_end>
   100f8:	ff010113          	addi	sp,sp,-16
   100fc:	fb5ff0ef          	jal	ra,100b0 <main>
   10100:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe38>
   10104:	00000013          	nop
   10108:	00000013          	nop
   1010c:	00000013          	nop
   10110:	00000013          	nop
   10114:	00000013          	nop
   10118:	00000013          	nop
