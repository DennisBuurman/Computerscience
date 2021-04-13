
comp.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	000116b7          	lui	a3,0x11
   100b4:	ff010113          	addi	sp,sp,-16
   100b8:	21868713          	addi	a4,a3,536 # 11218 <B>
   100bc:	00113423          	sd	ra,8(sp)
   100c0:	21868693          	addi	a3,a3,536
   100c4:	00000793          	li	a5,0
   100c8:	01400613          	li	a2,20
   100cc:	00f6a023          	sw	a5,0(a3)
   100d0:	0017879b          	addiw	a5,a5,1
   100d4:	00468693          	addi	a3,a3,4
   100d8:	fec79ae3          	bne	a5,a2,100cc <main+0x1c>
   100dc:	000116b7          	lui	a3,0x11
   100e0:	1cc68693          	addi	a3,a3,460 # 111cc <A+0x4>
   100e4:	04c70613          	addi	a2,a4,76
   100e8:	00072783          	lw	a5,0(a4)
   100ec:	00468693          	addi	a3,a3,4
   100f0:	00470713          	addi	a4,a4,4
   100f4:	02a7879b          	addiw	a5,a5,42
   100f8:	fef6ae23          	sw	a5,-4(a3)
   100fc:	fec716e3          	bne	a4,a2,100e8 <main+0x38>
   10100:	84c1a503          	lw	a0,-1972(gp) # 11214 <A+0x4c>
   10104:	014000ef          	jal	ra,10118 <print_roman>
   10108:	00813083          	ld	ra,8(sp)
   1010c:	00000513          	li	a0,0
   10110:	01010113          	addi	sp,sp,16
   10114:	00008067          	ret

0000000000010118 <print_roman>:
   10118:	00010737          	lui	a4,0x10
   1011c:	18870713          	addi	a4,a4,392 # 10188 <roman_table>
   10120:	3e800793          	li	a5,1000
   10124:	00f54c63          	blt	a0,a5,1013c <print_roman+0x24>
   10128:	00474783          	lbu	a5,4(a4)
   1012c:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   10130:	00072783          	lw	a5,0(a4)
   10134:	40f5053b          	subw	a0,a0,a5
   10138:	fef558e3          	ble	a5,a0,10128 <print_roman+0x10>
   1013c:	00870713          	addi	a4,a4,8
   10140:	00072783          	lw	a5,0(a4)
   10144:	fe0790e3          	bnez	a5,10124 <print_roman+0xc>
   10148:	00a00793          	li	a5,10
   1014c:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   10150:	00008067          	ret

0000000000010154 <_start>:
   10154:	00002197          	auipc	gp,0x2
   10158:	87418193          	addi	gp,gp,-1932 # 119c8 <__global_pointer$>
   1015c:	00012137          	lui	sp,0x12
   10160:	26810113          	addi	sp,sp,616 # 12268 <_end>
   10164:	ff010113          	addi	sp,sp,-16
   10168:	f49ff0ef          	jal	ra,100b0 <main>
   1016c:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe38>
   10170:	00000013          	nop
   10174:	00000013          	nop
   10178:	00000013          	nop
   1017c:	00000013          	nop
   10180:	00000013          	nop
   10184:	00000013          	nop
