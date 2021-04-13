
matvec.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	fe010113          	addi	sp,sp,-32
   100b4:	00011537          	lui	a0,0x11
   100b8:	00813823          	sd	s0,16(sp)
   100bc:	00010837          	lui	a6,0x10
   100c0:	21050413          	addi	s0,a0,528 # 11210 <A>
   100c4:	000105b7          	lui	a1,0x10
   100c8:	00113c23          	sd	ra,24(sp)
   100cc:	00913423          	sd	s1,8(sp)
   100d0:	02040893          	addi	a7,s0,32
   100d4:	21050513          	addi	a0,a0,528
   100d8:	1b080813          	addi	a6,a6,432 # 101b0 <B>
   100dc:	1d058593          	addi	a1,a1,464 # 101d0 <roman_table>
   100e0:	00080713          	mv	a4,a6
   100e4:	00050793          	mv	a5,a0
   100e8:	02200613          	li	a2,34
   100ec:	0080006f          	j	100f4 <main+0x44>
   100f0:	00072603          	lw	a2,0(a4)
   100f4:	0007a683          	lw	a3,0(a5)
   100f8:	02078793          	addi	a5,a5,32
   100fc:	00470713          	addi	a4,a4,4
   10100:	00c686bb          	addw	a3,a3,a2
   10104:	fed7a023          	sw	a3,-32(a5)
   10108:	feb714e3          	bne	a4,a1,100f0 <main+0x40>
   1010c:	00450513          	addi	a0,a0,4
   10110:	fd1518e3          	bne	a0,a7,100e0 <main+0x30>
   10114:	12040493          	addi	s1,s0,288
   10118:	00042503          	lw	a0,0(s0)
   1011c:	02440413          	addi	s0,s0,36
   10120:	020000ef          	jal	ra,10140 <print_roman>
   10124:	fe941ae3          	bne	s0,s1,10118 <main+0x68>
   10128:	01813083          	ld	ra,24(sp)
   1012c:	01013403          	ld	s0,16(sp)
   10130:	00813483          	ld	s1,8(sp)
   10134:	00000513          	li	a0,0
   10138:	02010113          	addi	sp,sp,32
   1013c:	00008067          	ret

0000000000010140 <print_roman>:
   10140:	00010737          	lui	a4,0x10
   10144:	1d070713          	addi	a4,a4,464 # 101d0 <roman_table>
   10148:	3e800793          	li	a5,1000
   1014c:	00f54c63          	blt	a0,a5,10164 <print_roman+0x24>
   10150:	00474783          	lbu	a5,4(a4)
   10154:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   10158:	00072783          	lw	a5,0(a4)
   1015c:	40f5053b          	subw	a0,a0,a5
   10160:	fef558e3          	ble	a5,a0,10150 <print_roman+0x10>
   10164:	00870713          	addi	a4,a4,8
   10168:	00072783          	lw	a5,0(a4)
   1016c:	fe0790e3          	bnez	a5,1014c <print_roman+0xc>
   10170:	00a00793          	li	a5,10
   10174:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   10178:	00008067          	ret

000000000001017c <_start>:
   1017c:	00002197          	auipc	gp,0x2
   10180:	99418193          	addi	gp,gp,-1644 # 11b10 <__global_pointer$>
   10184:	00012137          	lui	sp,0x12
   10188:	31010113          	addi	sp,sp,784 # 12310 <_end>
   1018c:	ff010113          	addi	sp,sp,-16
   10190:	f21ff0ef          	jal	ra,100b0 <main>
   10194:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe38>
   10198:	00000013          	nop
   1019c:	00000013          	nop
   101a0:	00000013          	nop
   101a4:	00000013          	nop
   101a8:	00000013          	nop
   101ac:	00000013          	nop
