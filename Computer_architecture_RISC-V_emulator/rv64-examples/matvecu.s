
matvecu.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <main>:
   100b0:	000117b7          	lui	a5,0x11
   100b4:	24078793          	addi	a5,a5,576 # 11240 <B>
   100b8:	fe010113          	addi	sp,sp,-32
   100bc:	00813823          	sd	s0,16(sp)
   100c0:	02078413          	addi	s0,a5,32
   100c4:	00113c23          	sd	ra,24(sp)
   100c8:	00913423          	sd	s1,8(sp)
   100cc:	01213023          	sd	s2,0(sp)
   100d0:	0047a083          	lw	ra,4(a5)
   100d4:	0007a903          	lw	s2,0(a5)
   100d8:	0087a483          	lw	s1,8(a5)
   100dc:	00c7a383          	lw	t2,12(a5)
   100e0:	0107a283          	lw	t0,16(a5)
   100e4:	0147af83          	lw	t6,20(a5)
   100e8:	0187af03          	lw	t5,24(a5)
   100ec:	01c7ae83          	lw	t4,28(a5)
   100f0:	04078e13          	addi	t3,a5,64
   100f4:	00040793          	mv	a5,s0
   100f8:	0007a303          	lw	t1,0(a5)
   100fc:	0207a883          	lw	a7,32(a5)
   10100:	0407a803          	lw	a6,64(a5)
   10104:	0607a503          	lw	a0,96(a5)
   10108:	0807a583          	lw	a1,128(a5)
   1010c:	0a07a603          	lw	a2,160(a5)
   10110:	0c07a683          	lw	a3,192(a5)
   10114:	0e07a703          	lw	a4,224(a5)
   10118:	0123033b          	addw	t1,t1,s2
   1011c:	001888bb          	addw	a7,a7,ra
   10120:	0098083b          	addw	a6,a6,s1
   10124:	0075053b          	addw	a0,a0,t2
   10128:	005585bb          	addw	a1,a1,t0
   1012c:	01f6063b          	addw	a2,a2,t6
   10130:	01e686bb          	addw	a3,a3,t5
   10134:	01d7073b          	addw	a4,a4,t4
   10138:	0067a023          	sw	t1,0(a5)
   1013c:	0317a023          	sw	a7,32(a5)
   10140:	0507a023          	sw	a6,64(a5)
   10144:	06a7a023          	sw	a0,96(a5)
   10148:	08b7a023          	sw	a1,128(a5)
   1014c:	0ac7a023          	sw	a2,160(a5)
   10150:	0cd7a023          	sw	a3,192(a5)
   10154:	0ee7a023          	sw	a4,224(a5)
   10158:	00478793          	addi	a5,a5,4
   1015c:	f8fe1ee3          	bne	t3,a5,100f8 <main+0x48>
   10160:	12040493          	addi	s1,s0,288
   10164:	00042503          	lw	a0,0(s0)
   10168:	02440413          	addi	s0,s0,36
   1016c:	024000ef          	jal	ra,10190 <print_roman>
   10170:	fe849ae3          	bne	s1,s0,10164 <main+0xb4>
   10174:	01813083          	ld	ra,24(sp)
   10178:	01013403          	ld	s0,16(sp)
   1017c:	00813483          	ld	s1,8(sp)
   10180:	00013903          	ld	s2,0(sp)
   10184:	00000513          	li	a0,0
   10188:	02010113          	addi	sp,sp,32
   1018c:	00008067          	ret

0000000000010190 <print_roman>:
   10190:	00010737          	lui	a4,0x10
   10194:	20070713          	addi	a4,a4,512 # 10200 <roman_table>
   10198:	3e800793          	li	a5,1000
   1019c:	00f54c63          	blt	a0,a5,101b4 <print_roman+0x24>
   101a0:	00474783          	lbu	a5,4(a4)
   101a4:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   101a8:	00072783          	lw	a5,0(a4)
   101ac:	40f5053b          	subw	a0,a0,a5
   101b0:	fef558e3          	ble	a5,a0,101a0 <print_roman+0x10>
   101b4:	00870713          	addi	a4,a4,8
   101b8:	00072783          	lw	a5,0(a4)
   101bc:	fe0790e3          	bnez	a5,1019c <print_roman+0xc>
   101c0:	00a00793          	li	a5,10
   101c4:	20f00023          	sb	a5,512(zero) # 200 <main-0xfeb0>
   101c8:	00008067          	ret

00000000000101cc <_start>:
   101cc:	00002197          	auipc	gp,0x2
   101d0:	99418193          	addi	gp,gp,-1644 # 11b60 <__global_pointer$>
   101d4:	00012137          	lui	sp,0x12
   101d8:	36010113          	addi	sp,sp,864 # 12360 <_end>
   101dc:	ff010113          	addi	sp,sp,-16
   101e0:	ed1ff0ef          	jal	ra,100b0 <main>
   101e4:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe38>
   101e8:	00000013          	nop
   101ec:	00000013          	nop
   101f0:	00000013          	nop
   101f4:	00000013          	nop
   101f8:	00000013          	nop
   101fc:	00000013          	nop
