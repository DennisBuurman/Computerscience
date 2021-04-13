
mandel.bin:     file format elf64-littleriscv


Disassembly of section .text:

0000000000010078 <main>:
   10078:	f6010113          	addi	sp,sp,-160
   1007c:	000137b7          	lui	a5,0x13
   10080:	07413823          	sd	s4,112(sp)
   10084:	b407aa03          	lw	s4,-1216(a5) # 12b40 <__clz_tab+0x1118>
   10088:	000137b7          	lui	a5,0x13
   1008c:	b287b783          	ld	a5,-1240(a5) # 12b28 <__clz_tab+0x1100>
   10090:	07513423          	sd	s5,104(sp)
   10094:	07613023          	sd	s6,96(sp)
   10098:	00f13c23          	sd	a5,24(sp)
   1009c:	000137b7          	lui	a5,0x13
   100a0:	b447a783          	lw	a5,-1212(a5) # 12b44 <__clz_tab+0x111c>
   100a4:	05813823          	sd	s8,80(sp)
   100a8:	03b13c23          	sd	s11,56(sp)
   100ac:	02f12223          	sw	a5,36(sp)
   100b0:	000137b7          	lui	a5,0x13
   100b4:	b487aa83          	lw	s5,-1208(a5) # 12b48 <__clz_tab+0x1120>
   100b8:	000137b7          	lui	a5,0x13
   100bc:	b307b783          	ld	a5,-1232(a5) # 12b30 <__clz_tab+0x1108>
   100c0:	08113c23          	sd	ra,152(sp)
   100c4:	08813823          	sd	s0,144(sp)
   100c8:	00f13423          	sd	a5,8(sp)
   100cc:	000137b7          	lui	a5,0x13
   100d0:	b387b783          	ld	a5,-1224(a5) # 12b38 <__clz_tab+0x1110>
   100d4:	08913423          	sd	s1,136(sp)
   100d8:	09213023          	sd	s2,128(sp)
   100dc:	02f13423          	sd	a5,40(sp)
   100e0:	04c00793          	li	a5,76
   100e4:	07313c23          	sd	s3,120(sp)
   100e8:	05713c23          	sd	s7,88(sp)
   100ec:	05913423          	sd	s9,72(sp)
   100f0:	05a13023          	sd	s10,64(sp)
   100f4:	00f13823          	sd	a5,16(sp)
   100f8:	00000c13          	li	s8,0
   100fc:	00000d93          	li	s11,0
   10100:	02000b13          	li	s6,32
   10104:	01013783          	ld	a5,16(sp)
   10108:	02412983          	lw	s3,36(sp)
   1010c:	fb478b9b          	addiw	s7,a5,-76
   10110:	00078d1b          	sext.w	s10,a5
   10114:	001b8b9b          	addiw	s7,s7,1
   10118:	04c00793          	li	a5,76
   1011c:	02fbe7bb          	remw	a5,s7,a5
   10120:	12079a63          	bnez	a5,10254 <main+0x1dc>
   10124:	00d00793          	li	a5,13
   10128:	20f00023          	sb	a5,512(zero) # 200 <main-0xfe78>
   1012c:	00a00793          	li	a5,10
   10130:	20f00023          	sb	a5,512(zero) # 200 <main-0xfe78>
   10134:	00000413          	li	s0,0
   10138:	07e00c13          	li	s8,126
   1013c:	00040c93          	mv	s9,s0
   10140:	00040913          	mv	s2,s0
   10144:	0100006f          	j	10154 <main+0xdc>
   10148:	fffc0c1b          	addiw	s8,s8,-1
   1014c:	00048d93          	mv	s11,s1
   10150:	076c0a63          	beq	s8,s6,101c4 <main+0x14c>
   10154:	000c8593          	mv	a1,s9
   10158:	00090513          	mv	a0,s2
   1015c:	1f8010ef          	jal	ra,11354 <__subsf3>
   10160:	00098593          	mv	a1,s3
   10164:	221000ef          	jal	ra,10b84 <__addsf3>
   10168:	00050493          	mv	s1,a0
   1016c:	000d8593          	mv	a1,s11
   10170:	000d8513          	mv	a0,s11
   10174:	211000ef          	jal	ra,10b84 <__addsf3>
   10178:	00040593          	mv	a1,s0
   1017c:	72d000ef          	jal	ra,110a8 <__mulsf3>
   10180:	000a0593          	mv	a1,s4
   10184:	201000ef          	jal	ra,10b84 <__addsf3>
   10188:	00050413          	mv	s0,a0
   1018c:	00048593          	mv	a1,s1
   10190:	00048513          	mv	a0,s1
   10194:	715000ef          	jal	ra,110a8 <__mulsf3>
   10198:	00050913          	mv	s2,a0
   1019c:	00040593          	mv	a1,s0
   101a0:	00040513          	mv	a0,s0
   101a4:	705000ef          	jal	ra,110a8 <__mulsf3>
   101a8:	00050593          	mv	a1,a0
   101ac:	00050c93          	mv	s9,a0
   101b0:	00090513          	mv	a0,s2
   101b4:	1d1000ef          	jal	ra,10b84 <__addsf3>
   101b8:	000a8593          	mv	a1,s5
   101bc:	621000ef          	jal	ra,10fdc <__lesf2>
   101c0:	f80544e3          	bltz	a0,10148 <main+0xd0>
   101c4:	00098513          	mv	a0,s3
   101c8:	604010ef          	jal	ra,117cc <__extendsfdf2>
   101cc:	00813583          	ld	a1,8(sp)
   101d0:	00048d93          	mv	s11,s1
   101d4:	0c0000ef          	jal	ra,10294 <__adddf3>
   101d8:	69c010ef          	jal	ra,11874 <__truncdfsf2>
   101dc:	00050993          	mv	s3,a0
   101e0:	f37d1ae3          	bne	s10,s7,10114 <main+0x9c>
   101e4:	02813583          	ld	a1,40(sp)
   101e8:	01813503          	ld	a0,24(sp)
   101ec:	510000ef          	jal	ra,106fc <__subdf3>
   101f0:	684010ef          	jal	ra,11874 <__truncdfsf2>
   101f4:	01013783          	ld	a5,16(sp)
   101f8:	00050a13          	mv	s4,a0
   101fc:	04c7841b          	addiw	s0,a5,76
   10200:	00813823          	sd	s0,16(sp)
   10204:	5c8010ef          	jal	ra,117cc <__extendsfdf2>
   10208:	00a13c23          	sd	a0,24(sp)
   1020c:	72000793          	li	a5,1824
   10210:	eef41ae3          	bne	s0,a5,10104 <main+0x8c>
   10214:	09813083          	ld	ra,152(sp)
   10218:	09013403          	ld	s0,144(sp)
   1021c:	08813483          	ld	s1,136(sp)
   10220:	08013903          	ld	s2,128(sp)
   10224:	07813983          	ld	s3,120(sp)
   10228:	07013a03          	ld	s4,112(sp)
   1022c:	06813a83          	ld	s5,104(sp)
   10230:	06013b03          	ld	s6,96(sp)
   10234:	05813b83          	ld	s7,88(sp)
   10238:	05013c03          	ld	s8,80(sp)
   1023c:	04813c83          	ld	s9,72(sp)
   10240:	04013d03          	ld	s10,64(sp)
   10244:	03813d83          	ld	s11,56(sp)
   10248:	00000513          	li	a0,0
   1024c:	0a010113          	addi	sp,sp,160
   10250:	00008067          	ret
   10254:	0ffc7713          	andi	a4,s8,255
   10258:	20e00023          	sb	a4,512(zero) # 200 <main-0xfe78>
   1025c:	ed9ff06f          	j	10134 <main+0xbc>

0000000000010260 <_start>:
   10260:	00003197          	auipc	gp,0x3
   10264:	0c818193          	addi	gp,gp,200 # 13328 <__global_pointer$>
   10268:	00014137          	lui	sp,0x14
   1026c:	b5010113          	addi	sp,sp,-1200 # 13b50 <_end>
   10270:	ff010113          	addi	sp,sp,-16
   10274:	e05ff0ef          	jal	ra,10078 <main>
   10278:	26a02c23          	sw	a0,632(zero) # 278 <main-0xfe00>
   1027c:	00000013          	nop
   10280:	00000013          	nop
   10284:	00000013          	nop
   10288:	00000013          	nop
   1028c:	00000013          	nop
   10290:	00000013          	nop

0000000000010294 <__adddf3>:
   10294:	fe010113          	addi	sp,sp,-32
   10298:	fff00813          	li	a6,-1
   1029c:	00c85613          	srli	a2,a6,0xc
   102a0:	0345d693          	srli	a3,a1,0x34
   102a4:	00813823          	sd	s0,16(sp)
   102a8:	03455413          	srli	s0,a0,0x34
   102ac:	00a677b3          	and	a5,a2,a0
   102b0:	01213023          	sd	s2,0(sp)
   102b4:	00b67633          	and	a2,a2,a1
   102b8:	7ff47413          	andi	s0,s0,2047
   102bc:	7ff6f693          	andi	a3,a3,2047
   102c0:	00113c23          	sd	ra,24(sp)
   102c4:	00913423          	sd	s1,8(sp)
   102c8:	03f55913          	srli	s2,a0,0x3f
   102cc:	03f5d593          	srli	a1,a1,0x3f
   102d0:	00379793          	slli	a5,a5,0x3
   102d4:	00361613          	slli	a2,a2,0x3
   102d8:	40d4073b          	subw	a4,s0,a3
   102dc:	18b91a63          	bne	s2,a1,10470 <__adddf3+0x1dc>
   102e0:	0007059b          	sext.w	a1,a4
   102e4:	0ab05863          	blez	a1,10394 <__adddf3+0x100>
   102e8:	06069663          	bnez	a3,10354 <__adddf3+0xc0>
   102ec:	04060663          	beqz	a2,10338 <__adddf3+0xa4>
   102f0:	fff7059b          	addiw	a1,a4,-1
   102f4:	02059e63          	bnez	a1,10330 <__adddf3+0x9c>
   102f8:	00c787b3          	add	a5,a5,a2
   102fc:	00879713          	slli	a4,a5,0x8
   10300:	02075c63          	bgez	a4,10338 <__adddf3+0xa4>
   10304:	00140413          	addi	s0,s0,1
   10308:	7ff00713          	li	a4,2047
   1030c:	34e40463          	beq	s0,a4,10654 <__adddf3+0x3c0>
   10310:	1ff00713          	li	a4,511
   10314:	03671713          	slli	a4,a4,0x36
   10318:	0017f693          	andi	a3,a5,1
   1031c:	fff70713          	addi	a4,a4,-1
   10320:	0017d793          	srli	a5,a5,0x1
   10324:	00e7f7b3          	and	a5,a5,a4
   10328:	00d7e7b3          	or	a5,a5,a3
   1032c:	00c0006f          	j	10338 <__adddf3+0xa4>
   10330:	7ff00713          	li	a4,2047
   10334:	02e41a63          	bne	s0,a4,10368 <__adddf3+0xd4>
   10338:	0077f713          	andi	a4,a5,7
   1033c:	30070e63          	beqz	a4,10658 <__adddf3+0x3c4>
   10340:	00f7f713          	andi	a4,a5,15
   10344:	00400693          	li	a3,4
   10348:	30d70863          	beq	a4,a3,10658 <__adddf3+0x3c4>
   1034c:	00478793          	addi	a5,a5,4
   10350:	3080006f          	j	10658 <__adddf3+0x3c4>
   10354:	7ff00713          	li	a4,2047
   10358:	fee400e3          	beq	s0,a4,10338 <__adddf3+0xa4>
   1035c:	00100713          	li	a4,1
   10360:	03771713          	slli	a4,a4,0x37
   10364:	00e66633          	or	a2,a2,a4
   10368:	03800713          	li	a4,56
   1036c:	00b75663          	ble	a1,a4,10378 <__adddf3+0xe4>
   10370:	00100613          	li	a2,1
   10374:	f85ff06f          	j	102f8 <__adddf3+0x64>
   10378:	04000713          	li	a4,64
   1037c:	00b656b3          	srl	a3,a2,a1
   10380:	40b705bb          	subw	a1,a4,a1
   10384:	00b61633          	sll	a2,a2,a1
   10388:	00c03633          	snez	a2,a2
   1038c:	00c6e633          	or	a2,a3,a2
   10390:	f69ff06f          	j	102f8 <__adddf3+0x64>
   10394:	06058863          	beqz	a1,10404 <__adddf3+0x170>
   10398:	02041463          	bnez	s0,103c0 <__adddf3+0x12c>
   1039c:	2a078063          	beqz	a5,1063c <__adddf3+0x3a8>
   103a0:	01059863          	bne	a1,a6,103b0 <__adddf3+0x11c>
   103a4:	00c787b3          	add	a5,a5,a2
   103a8:	00068413          	mv	s0,a3
   103ac:	f51ff06f          	j	102fc <__adddf3+0x68>
   103b0:	7ff00593          	li	a1,2047
   103b4:	32b69863          	bne	a3,a1,106e4 <__adddf3+0x450>
   103b8:	00060793          	mv	a5,a2
   103bc:	2000006f          	j	105bc <__adddf3+0x328>
   103c0:	7ff00593          	li	a1,2047
   103c4:	feb68ae3          	beq	a3,a1,103b8 <__adddf3+0x124>
   103c8:	00100593          	li	a1,1
   103cc:	03759593          	slli	a1,a1,0x37
   103d0:	40e0073b          	negw	a4,a4
   103d4:	00b7e7b3          	or	a5,a5,a1
   103d8:	03800593          	li	a1,56
   103dc:	00e5d663          	ble	a4,a1,103e8 <__adddf3+0x154>
   103e0:	00100793          	li	a5,1
   103e4:	fc1ff06f          	j	103a4 <__adddf3+0x110>
   103e8:	04000593          	li	a1,64
   103ec:	00e7d533          	srl	a0,a5,a4
   103f0:	40e5873b          	subw	a4,a1,a4
   103f4:	00e797b3          	sll	a5,a5,a4
   103f8:	00f037b3          	snez	a5,a5
   103fc:	00f567b3          	or	a5,a0,a5
   10400:	fa5ff06f          	j	103a4 <__adddf3+0x110>
   10404:	00140713          	addi	a4,s0,1
   10408:	7fe77693          	andi	a3,a4,2046
   1040c:	04069663          	bnez	a3,10458 <__adddf3+0x1c4>
   10410:	02041663          	bnez	s0,1043c <__adddf3+0x1a8>
   10414:	22078a63          	beqz	a5,10648 <__adddf3+0x3b4>
   10418:	f20600e3          	beqz	a2,10338 <__adddf3+0xa4>
   1041c:	00c787b3          	add	a5,a5,a2
   10420:	00879713          	slli	a4,a5,0x8
   10424:	f0075ae3          	bgez	a4,10338 <__adddf3+0xa4>
   10428:	03781813          	slli	a6,a6,0x37
   1042c:	fff80813          	addi	a6,a6,-1
   10430:	0107f7b3          	and	a5,a5,a6
   10434:	00100413          	li	s0,1
   10438:	f01ff06f          	j	10338 <__adddf3+0xa4>
   1043c:	f6078ee3          	beqz	a5,103b8 <__adddf3+0x124>
   10440:	16060e63          	beqz	a2,105bc <__adddf3+0x328>
   10444:	00100793          	li	a5,1
   10448:	00000913          	li	s2,0
   1044c:	03679793          	slli	a5,a5,0x36
   10450:	7ff00413          	li	s0,2047
   10454:	2040006f          	j	10658 <__adddf3+0x3c4>
   10458:	7ff00693          	li	a3,2047
   1045c:	1ed70a63          	beq	a4,a3,10650 <__adddf3+0x3bc>
   10460:	00c787b3          	add	a5,a5,a2
   10464:	0017d793          	srli	a5,a5,0x1
   10468:	00070413          	mv	s0,a4
   1046c:	ecdff06f          	j	10338 <__adddf3+0xa4>
   10470:	0007051b          	sext.w	a0,a4
   10474:	06a05c63          	blez	a0,104ec <__adddf3+0x258>
   10478:	04069063          	bnez	a3,104b8 <__adddf3+0x224>
   1047c:	ea060ee3          	beqz	a2,10338 <__adddf3+0xa4>
   10480:	fff7051b          	addiw	a0,a4,-1
   10484:	00051e63          	bnez	a0,104a0 <__adddf3+0x20c>
   10488:	40c787b3          	sub	a5,a5,a2
   1048c:	00879713          	slli	a4,a5,0x8
   10490:	ea0754e3          	bgez	a4,10338 <__adddf3+0xa4>
   10494:	00979493          	slli	s1,a5,0x9
   10498:	0094d493          	srli	s1,s1,0x9
   1049c:	13c0006f          	j	105d8 <__adddf3+0x344>
   104a0:	7ff00713          	li	a4,2047
   104a4:	e8e40ae3          	beq	s0,a4,10338 <__adddf3+0xa4>
   104a8:	03800713          	li	a4,56
   104ac:	02a75263          	ble	a0,a4,104d0 <__adddf3+0x23c>
   104b0:	00100613          	li	a2,1
   104b4:	fd5ff06f          	j	10488 <__adddf3+0x1f4>
   104b8:	7ff00713          	li	a4,2047
   104bc:	e6e40ee3          	beq	s0,a4,10338 <__adddf3+0xa4>
   104c0:	00100713          	li	a4,1
   104c4:	03771713          	slli	a4,a4,0x37
   104c8:	00e66633          	or	a2,a2,a4
   104cc:	fddff06f          	j	104a8 <__adddf3+0x214>
   104d0:	04000713          	li	a4,64
   104d4:	00a656b3          	srl	a3,a2,a0
   104d8:	40a7053b          	subw	a0,a4,a0
   104dc:	00a61633          	sll	a2,a2,a0
   104e0:	00c03633          	snez	a2,a2
   104e4:	00c6e633          	or	a2,a3,a2
   104e8:	fa1ff06f          	j	10488 <__adddf3+0x1f4>
   104ec:	06050c63          	beqz	a0,10564 <__adddf3+0x2d0>
   104f0:	02041863          	bnez	s0,10520 <__adddf3+0x28c>
   104f4:	1c078c63          	beqz	a5,106cc <__adddf3+0x438>
   104f8:	01051a63          	bne	a0,a6,1050c <__adddf3+0x278>
   104fc:	40f607b3          	sub	a5,a2,a5
   10500:	00068413          	mv	s0,a3
   10504:	00058913          	mv	s2,a1
   10508:	f85ff06f          	j	1048c <__adddf3+0x1f8>
   1050c:	7ff00513          	li	a0,2047
   10510:	1ea69063          	bne	a3,a0,106f0 <__adddf3+0x45c>
   10514:	00060793          	mv	a5,a2
   10518:	7ff00413          	li	s0,2047
   1051c:	1b80006f          	j	106d4 <__adddf3+0x440>
   10520:	7ff00513          	li	a0,2047
   10524:	fea688e3          	beq	a3,a0,10514 <__adddf3+0x280>
   10528:	00100513          	li	a0,1
   1052c:	03751513          	slli	a0,a0,0x37
   10530:	40e0073b          	negw	a4,a4
   10534:	00a7e7b3          	or	a5,a5,a0
   10538:	03800513          	li	a0,56
   1053c:	00e55663          	ble	a4,a0,10548 <__adddf3+0x2b4>
   10540:	00100793          	li	a5,1
   10544:	fb9ff06f          	j	104fc <__adddf3+0x268>
   10548:	04000513          	li	a0,64
   1054c:	00e7d833          	srl	a6,a5,a4
   10550:	40e5073b          	subw	a4,a0,a4
   10554:	00e797b3          	sll	a5,a5,a4
   10558:	00f037b3          	snez	a5,a5
   1055c:	00f867b3          	or	a5,a6,a5
   10560:	f9dff06f          	j	104fc <__adddf3+0x268>
   10564:	00140713          	addi	a4,s0,1
   10568:	7fe77713          	andi	a4,a4,2046
   1056c:	04071c63          	bnez	a4,105c4 <__adddf3+0x330>
   10570:	02041e63          	bnez	s0,105ac <__adddf3+0x318>
   10574:	00079c63          	bnez	a5,1058c <__adddf3+0x2f8>
   10578:	00060793          	mv	a5,a2
   1057c:	14061c63          	bnez	a2,106d4 <__adddf3+0x440>
   10580:	00000793          	li	a5,0
   10584:	00000913          	li	s2,0
   10588:	0d00006f          	j	10658 <__adddf3+0x3c4>
   1058c:	da0606e3          	beqz	a2,10338 <__adddf3+0xa4>
   10590:	40c78733          	sub	a4,a5,a2
   10594:	00871693          	slli	a3,a4,0x8
   10598:	40f607b3          	sub	a5,a2,a5
   1059c:	1206cc63          	bltz	a3,106d4 <__adddf3+0x440>
   105a0:	fe0700e3          	beqz	a4,10580 <__adddf3+0x2ec>
   105a4:	00070793          	mv	a5,a4
   105a8:	d91ff06f          	j	10338 <__adddf3+0xa4>
   105ac:	e8079ae3          	bnez	a5,10440 <__adddf3+0x1ac>
   105b0:	e8060ae3          	beqz	a2,10444 <__adddf3+0x1b0>
   105b4:	00060793          	mv	a5,a2
   105b8:	00058913          	mv	s2,a1
   105bc:	7ff00413          	li	s0,2047
   105c0:	d79ff06f          	j	10338 <__adddf3+0xa4>
   105c4:	40c784b3          	sub	s1,a5,a2
   105c8:	00849713          	slli	a4,s1,0x8
   105cc:	04075463          	bgez	a4,10614 <__adddf3+0x380>
   105d0:	40f604b3          	sub	s1,a2,a5
   105d4:	00058913          	mv	s2,a1
   105d8:	00048513          	mv	a0,s1
   105dc:	3cc010ef          	jal	ra,119a8 <__clzdi2>
   105e0:	ff85079b          	addiw	a5,a0,-8
   105e4:	00f494b3          	sll	s1,s1,a5
   105e8:	0287ce63          	blt	a5,s0,10624 <__adddf3+0x390>
   105ec:	4087843b          	subw	s0,a5,s0
   105f0:	0014041b          	addiw	s0,s0,1
   105f4:	04000513          	li	a0,64
   105f8:	0084d7b3          	srl	a5,s1,s0
   105fc:	4085043b          	subw	s0,a0,s0
   10600:	008494b3          	sll	s1,s1,s0
   10604:	009034b3          	snez	s1,s1
   10608:	0097e7b3          	or	a5,a5,s1
   1060c:	00000413          	li	s0,0
   10610:	d29ff06f          	j	10338 <__adddf3+0xa4>
   10614:	fc0492e3          	bnez	s1,105d8 <__adddf3+0x344>
   10618:	00000793          	li	a5,0
   1061c:	00000413          	li	s0,0
   10620:	f65ff06f          	j	10584 <__adddf3+0x2f0>
   10624:	40f40433          	sub	s0,s0,a5
   10628:	fff00793          	li	a5,-1
   1062c:	03779793          	slli	a5,a5,0x37
   10630:	fff78793          	addi	a5,a5,-1
   10634:	00f4f7b3          	and	a5,s1,a5
   10638:	d01ff06f          	j	10338 <__adddf3+0xa4>
   1063c:	00060793          	mv	a5,a2
   10640:	00068413          	mv	s0,a3
   10644:	cf5ff06f          	j	10338 <__adddf3+0xa4>
   10648:	00060793          	mv	a5,a2
   1064c:	cedff06f          	j	10338 <__adddf3+0xa4>
   10650:	7ff00413          	li	s0,2047
   10654:	00000793          	li	a5,0
   10658:	00879713          	slli	a4,a5,0x8
   1065c:	02075063          	bgez	a4,1067c <__adddf3+0x3e8>
   10660:	00140413          	addi	s0,s0,1
   10664:	7ff00713          	li	a4,2047
   10668:	06e40a63          	beq	s0,a4,106dc <__adddf3+0x448>
   1066c:	fff00713          	li	a4,-1
   10670:	03771713          	slli	a4,a4,0x37
   10674:	fff70713          	addi	a4,a4,-1
   10678:	00e7f7b3          	and	a5,a5,a4
   1067c:	7ff00713          	li	a4,2047
   10680:	0037d793          	srli	a5,a5,0x3
   10684:	00e41a63          	bne	s0,a4,10698 <__adddf3+0x404>
   10688:	00078863          	beqz	a5,10698 <__adddf3+0x404>
   1068c:	00100793          	li	a5,1
   10690:	03379793          	slli	a5,a5,0x33
   10694:	00000913          	li	s2,0
   10698:	7ff47513          	andi	a0,s0,2047
   1069c:	01813083          	ld	ra,24(sp)
   106a0:	01013403          	ld	s0,16(sp)
   106a4:	00c79793          	slli	a5,a5,0xc
   106a8:	03451513          	slli	a0,a0,0x34
   106ac:	00c7d793          	srli	a5,a5,0xc
   106b0:	03f91913          	slli	s2,s2,0x3f
   106b4:	00a7e7b3          	or	a5,a5,a0
   106b8:	0127e533          	or	a0,a5,s2
   106bc:	00813483          	ld	s1,8(sp)
   106c0:	00013903          	ld	s2,0(sp)
   106c4:	02010113          	addi	sp,sp,32
   106c8:	00008067          	ret
   106cc:	00060793          	mv	a5,a2
   106d0:	00068413          	mv	s0,a3
   106d4:	00058913          	mv	s2,a1
   106d8:	c61ff06f          	j	10338 <__adddf3+0xa4>
   106dc:	00000793          	li	a5,0
   106e0:	f9dff06f          	j	1067c <__adddf3+0x3e8>
   106e4:	fff74713          	not	a4,a4
   106e8:	0007071b          	sext.w	a4,a4
   106ec:	cedff06f          	j	103d8 <__adddf3+0x144>
   106f0:	fff74713          	not	a4,a4
   106f4:	0007071b          	sext.w	a4,a4
   106f8:	e41ff06f          	j	10538 <__adddf3+0x2a4>

00000000000106fc <__subdf3>:
   106fc:	fff00813          	li	a6,-1
   10700:	fe010113          	addi	sp,sp,-32
   10704:	00c85813          	srli	a6,a6,0xc
   10708:	00a877b3          	and	a5,a6,a0
   1070c:	03455613          	srli	a2,a0,0x34
   10710:	01213023          	sd	s2,0(sp)
   10714:	03f55913          	srli	s2,a0,0x3f
   10718:	0345d513          	srli	a0,a1,0x34
   1071c:	00b87833          	and	a6,a6,a1
   10720:	00813823          	sd	s0,16(sp)
   10724:	7ff67613          	andi	a2,a2,2047
   10728:	00113c23          	sd	ra,24(sp)
   1072c:	00913423          	sd	s1,8(sp)
   10730:	7ff57513          	andi	a0,a0,2047
   10734:	7ff00713          	li	a4,2047
   10738:	00060413          	mv	s0,a2
   1073c:	00379793          	slli	a5,a5,0x3
   10740:	03f5d593          	srli	a1,a1,0x3f
   10744:	00381813          	slli	a6,a6,0x3
   10748:	00e51463          	bne	a0,a4,10750 <__subdf3+0x54>
   1074c:	00081463          	bnez	a6,10754 <__subdf3+0x58>
   10750:	0015c593          	xori	a1,a1,1
   10754:	40a6073b          	subw	a4,a2,a0
   10758:	0007069b          	sext.w	a3,a4
   1075c:	19259c63          	bne	a1,s2,108f4 <__subdf3+0x1f8>
   10760:	0ad05863          	blez	a3,10810 <__subdf3+0x114>
   10764:	06051663          	bnez	a0,107d0 <__subdf3+0xd4>
   10768:	04080663          	beqz	a6,107b4 <__subdf3+0xb8>
   1076c:	fff7069b          	addiw	a3,a4,-1
   10770:	02069e63          	bnez	a3,107ac <__subdf3+0xb0>
   10774:	010787b3          	add	a5,a5,a6
   10778:	00879713          	slli	a4,a5,0x8
   1077c:	02075c63          	bgez	a4,107b4 <__subdf3+0xb8>
   10780:	00140413          	addi	s0,s0,1
   10784:	7ff00713          	li	a4,2047
   10788:	34e40c63          	beq	s0,a4,10ae0 <__subdf3+0x3e4>
   1078c:	1ff00713          	li	a4,511
   10790:	03671713          	slli	a4,a4,0x36
   10794:	0017f693          	andi	a3,a5,1
   10798:	fff70713          	addi	a4,a4,-1
   1079c:	0017d793          	srli	a5,a5,0x1
   107a0:	00e7f7b3          	and	a5,a5,a4
   107a4:	00d7e7b3          	or	a5,a5,a3
   107a8:	00c0006f          	j	107b4 <__subdf3+0xb8>
   107ac:	7ff00713          	li	a4,2047
   107b0:	02e61a63          	bne	a2,a4,107e4 <__subdf3+0xe8>
   107b4:	0077f713          	andi	a4,a5,7
   107b8:	32070663          	beqz	a4,10ae4 <__subdf3+0x3e8>
   107bc:	00f7f713          	andi	a4,a5,15
   107c0:	00400693          	li	a3,4
   107c4:	32d70063          	beq	a4,a3,10ae4 <__subdf3+0x3e8>
   107c8:	00478793          	addi	a5,a5,4
   107cc:	3180006f          	j	10ae4 <__subdf3+0x3e8>
   107d0:	7ff00713          	li	a4,2047
   107d4:	fee600e3          	beq	a2,a4,107b4 <__subdf3+0xb8>
   107d8:	00100713          	li	a4,1
   107dc:	03771713          	slli	a4,a4,0x37
   107e0:	00e86833          	or	a6,a6,a4
   107e4:	03800713          	li	a4,56
   107e8:	00d75663          	ble	a3,a4,107f4 <__subdf3+0xf8>
   107ec:	00100813          	li	a6,1
   107f0:	f85ff06f          	j	10774 <__subdf3+0x78>
   107f4:	04000713          	li	a4,64
   107f8:	00d85633          	srl	a2,a6,a3
   107fc:	40d706bb          	subw	a3,a4,a3
   10800:	00d81833          	sll	a6,a6,a3
   10804:	01003833          	snez	a6,a6
   10808:	01066833          	or	a6,a2,a6
   1080c:	f69ff06f          	j	10774 <__subdf3+0x78>
   10810:	06068a63          	beqz	a3,10884 <__subdf3+0x188>
   10814:	02061663          	bnez	a2,10840 <__subdf3+0x144>
   10818:	2a078863          	beqz	a5,10ac8 <__subdf3+0x3cc>
   1081c:	fff00613          	li	a2,-1
   10820:	00c69863          	bne	a3,a2,10830 <__subdf3+0x134>
   10824:	010787b3          	add	a5,a5,a6
   10828:	00050413          	mv	s0,a0
   1082c:	f4dff06f          	j	10778 <__subdf3+0x7c>
   10830:	7ff00693          	li	a3,2047
   10834:	32d51c63          	bne	a0,a3,10b6c <__subdf3+0x470>
   10838:	00080793          	mv	a5,a6
   1083c:	2080006f          	j	10a44 <__subdf3+0x348>
   10840:	7ff00693          	li	a3,2047
   10844:	fed50ae3          	beq	a0,a3,10838 <__subdf3+0x13c>
   10848:	00100693          	li	a3,1
   1084c:	03769693          	slli	a3,a3,0x37
   10850:	40e0073b          	negw	a4,a4
   10854:	00d7e7b3          	or	a5,a5,a3
   10858:	03800693          	li	a3,56
   1085c:	00e6d663          	ble	a4,a3,10868 <__subdf3+0x16c>
   10860:	00100793          	li	a5,1
   10864:	fc1ff06f          	j	10824 <__subdf3+0x128>
   10868:	04000693          	li	a3,64
   1086c:	00e7d633          	srl	a2,a5,a4
   10870:	40e6873b          	subw	a4,a3,a4
   10874:	00e797b3          	sll	a5,a5,a4
   10878:	00f037b3          	snez	a5,a5
   1087c:	00f667b3          	or	a5,a2,a5
   10880:	fa5ff06f          	j	10824 <__subdf3+0x128>
   10884:	00160713          	addi	a4,a2,1
   10888:	7fe77693          	andi	a3,a4,2046
   1088c:	04069863          	bnez	a3,108dc <__subdf3+0x1e0>
   10890:	02061863          	bnez	a2,108c0 <__subdf3+0x1c4>
   10894:	24078063          	beqz	a5,10ad4 <__subdf3+0x3d8>
   10898:	f0080ee3          	beqz	a6,107b4 <__subdf3+0xb8>
   1089c:	010787b3          	add	a5,a5,a6
   108a0:	00879713          	slli	a4,a5,0x8
   108a4:	f00758e3          	bgez	a4,107b4 <__subdf3+0xb8>
   108a8:	fff00713          	li	a4,-1
   108ac:	03771713          	slli	a4,a4,0x37
   108b0:	fff70713          	addi	a4,a4,-1
   108b4:	00e7f7b3          	and	a5,a5,a4
   108b8:	00100413          	li	s0,1
   108bc:	ef9ff06f          	j	107b4 <__subdf3+0xb8>
   108c0:	f6078ce3          	beqz	a5,10838 <__subdf3+0x13c>
   108c4:	18080063          	beqz	a6,10a44 <__subdf3+0x348>
   108c8:	00100793          	li	a5,1
   108cc:	00000913          	li	s2,0
   108d0:	03679793          	slli	a5,a5,0x36
   108d4:	7ff00413          	li	s0,2047
   108d8:	20c0006f          	j	10ae4 <__subdf3+0x3e8>
   108dc:	7ff00693          	li	a3,2047
   108e0:	1ed70e63          	beq	a4,a3,10adc <__subdf3+0x3e0>
   108e4:	010787b3          	add	a5,a5,a6
   108e8:	0017d793          	srli	a5,a5,0x1
   108ec:	00070413          	mv	s0,a4
   108f0:	ec5ff06f          	j	107b4 <__subdf3+0xb8>
   108f4:	06d05c63          	blez	a3,1096c <__subdf3+0x270>
   108f8:	04051063          	bnez	a0,10938 <__subdf3+0x23c>
   108fc:	ea080ce3          	beqz	a6,107b4 <__subdf3+0xb8>
   10900:	fff7069b          	addiw	a3,a4,-1
   10904:	00069e63          	bnez	a3,10920 <__subdf3+0x224>
   10908:	410787b3          	sub	a5,a5,a6
   1090c:	00879713          	slli	a4,a5,0x8
   10910:	ea0752e3          	bgez	a4,107b4 <__subdf3+0xb8>
   10914:	00979493          	slli	s1,a5,0x9
   10918:	0094d493          	srli	s1,s1,0x9
   1091c:	1440006f          	j	10a60 <__subdf3+0x364>
   10920:	7ff00713          	li	a4,2047
   10924:	e8e608e3          	beq	a2,a4,107b4 <__subdf3+0xb8>
   10928:	03800713          	li	a4,56
   1092c:	02d75263          	ble	a3,a4,10950 <__subdf3+0x254>
   10930:	00100813          	li	a6,1
   10934:	fd5ff06f          	j	10908 <__subdf3+0x20c>
   10938:	7ff00713          	li	a4,2047
   1093c:	e6e60ce3          	beq	a2,a4,107b4 <__subdf3+0xb8>
   10940:	00100713          	li	a4,1
   10944:	03771713          	slli	a4,a4,0x37
   10948:	00e86833          	or	a6,a6,a4
   1094c:	fddff06f          	j	10928 <__subdf3+0x22c>
   10950:	04000713          	li	a4,64
   10954:	00d85633          	srl	a2,a6,a3
   10958:	40d706bb          	subw	a3,a4,a3
   1095c:	00d81833          	sll	a6,a6,a3
   10960:	01003833          	snez	a6,a6
   10964:	01066833          	or	a6,a2,a6
   10968:	fa1ff06f          	j	10908 <__subdf3+0x20c>
   1096c:	06068e63          	beqz	a3,109e8 <__subdf3+0x2ec>
   10970:	02061a63          	bnez	a2,109a4 <__subdf3+0x2a8>
   10974:	1e078263          	beqz	a5,10b58 <__subdf3+0x45c>
   10978:	fff00613          	li	a2,-1
   1097c:	00c69a63          	bne	a3,a2,10990 <__subdf3+0x294>
   10980:	40f807b3          	sub	a5,a6,a5
   10984:	00050413          	mv	s0,a0
   10988:	00058913          	mv	s2,a1
   1098c:	f81ff06f          	j	1090c <__subdf3+0x210>
   10990:	7ff00693          	li	a3,2047
   10994:	1ed51263          	bne	a0,a3,10b78 <__subdf3+0x47c>
   10998:	00080793          	mv	a5,a6
   1099c:	7ff00413          	li	s0,2047
   109a0:	0680006f          	j	10a08 <__subdf3+0x30c>
   109a4:	7ff00693          	li	a3,2047
   109a8:	fed508e3          	beq	a0,a3,10998 <__subdf3+0x29c>
   109ac:	00100693          	li	a3,1
   109b0:	03769693          	slli	a3,a3,0x37
   109b4:	40e0073b          	negw	a4,a4
   109b8:	00d7e7b3          	or	a5,a5,a3
   109bc:	03800693          	li	a3,56
   109c0:	00e6d663          	ble	a4,a3,109cc <__subdf3+0x2d0>
   109c4:	00100793          	li	a5,1
   109c8:	fb9ff06f          	j	10980 <__subdf3+0x284>
   109cc:	04000693          	li	a3,64
   109d0:	00e7d633          	srl	a2,a5,a4
   109d4:	40e6873b          	subw	a4,a3,a4
   109d8:	00e797b3          	sll	a5,a5,a4
   109dc:	00f037b3          	snez	a5,a5
   109e0:	00f667b3          	or	a5,a2,a5
   109e4:	f9dff06f          	j	10980 <__subdf3+0x284>
   109e8:	00160713          	addi	a4,a2,1
   109ec:	7fe77713          	andi	a4,a4,2046
   109f0:	04071e63          	bnez	a4,10a4c <__subdf3+0x350>
   109f4:	04061063          	bnez	a2,10a34 <__subdf3+0x338>
   109f8:	00079c63          	bnez	a5,10a10 <__subdf3+0x314>
   109fc:	00000913          	li	s2,0
   10a00:	0e080263          	beqz	a6,10ae4 <__subdf3+0x3e8>
   10a04:	00080793          	mv	a5,a6
   10a08:	00058913          	mv	s2,a1
   10a0c:	da9ff06f          	j	107b4 <__subdf3+0xb8>
   10a10:	da0802e3          	beqz	a6,107b4 <__subdf3+0xb8>
   10a14:	41078733          	sub	a4,a5,a6
   10a18:	00871693          	slli	a3,a4,0x8
   10a1c:	40f807b3          	sub	a5,a6,a5
   10a20:	fe06c4e3          	bltz	a3,10a08 <__subdf3+0x30c>
   10a24:	00000793          	li	a5,0
   10a28:	08070063          	beqz	a4,10aa8 <__subdf3+0x3ac>
   10a2c:	00070793          	mv	a5,a4
   10a30:	d85ff06f          	j	107b4 <__subdf3+0xb8>
   10a34:	e80798e3          	bnez	a5,108c4 <__subdf3+0x1c8>
   10a38:	e80808e3          	beqz	a6,108c8 <__subdf3+0x1cc>
   10a3c:	00080793          	mv	a5,a6
   10a40:	00058913          	mv	s2,a1
   10a44:	7ff00413          	li	s0,2047
   10a48:	d6dff06f          	j	107b4 <__subdf3+0xb8>
   10a4c:	410784b3          	sub	s1,a5,a6
   10a50:	00849713          	slli	a4,s1,0x8
   10a54:	04075463          	bgez	a4,10a9c <__subdf3+0x3a0>
   10a58:	40f804b3          	sub	s1,a6,a5
   10a5c:	00058913          	mv	s2,a1
   10a60:	00048513          	mv	a0,s1
   10a64:	745000ef          	jal	ra,119a8 <__clzdi2>
   10a68:	ff85079b          	addiw	a5,a0,-8
   10a6c:	00f494b3          	sll	s1,s1,a5
   10a70:	0487c063          	blt	a5,s0,10ab0 <__subdf3+0x3b4>
   10a74:	4087843b          	subw	s0,a5,s0
   10a78:	0014041b          	addiw	s0,s0,1
   10a7c:	04000513          	li	a0,64
   10a80:	0084d7b3          	srl	a5,s1,s0
   10a84:	4085043b          	subw	s0,a0,s0
   10a88:	008494b3          	sll	s1,s1,s0
   10a8c:	009034b3          	snez	s1,s1
   10a90:	0097e7b3          	or	a5,a5,s1
   10a94:	00000413          	li	s0,0
   10a98:	d1dff06f          	j	107b4 <__subdf3+0xb8>
   10a9c:	fc0492e3          	bnez	s1,10a60 <__subdf3+0x364>
   10aa0:	00000793          	li	a5,0
   10aa4:	00000413          	li	s0,0
   10aa8:	00000913          	li	s2,0
   10aac:	0380006f          	j	10ae4 <__subdf3+0x3e8>
   10ab0:	40f40433          	sub	s0,s0,a5
   10ab4:	fff00793          	li	a5,-1
   10ab8:	03779793          	slli	a5,a5,0x37
   10abc:	fff78793          	addi	a5,a5,-1
   10ac0:	00f4f7b3          	and	a5,s1,a5
   10ac4:	cf1ff06f          	j	107b4 <__subdf3+0xb8>
   10ac8:	00080793          	mv	a5,a6
   10acc:	00050413          	mv	s0,a0
   10ad0:	ce5ff06f          	j	107b4 <__subdf3+0xb8>
   10ad4:	00080793          	mv	a5,a6
   10ad8:	cddff06f          	j	107b4 <__subdf3+0xb8>
   10adc:	7ff00413          	li	s0,2047
   10ae0:	00000793          	li	a5,0
   10ae4:	00879713          	slli	a4,a5,0x8
   10ae8:	02075063          	bgez	a4,10b08 <__subdf3+0x40c>
   10aec:	00140413          	addi	s0,s0,1
   10af0:	7ff00713          	li	a4,2047
   10af4:	06e40863          	beq	s0,a4,10b64 <__subdf3+0x468>
   10af8:	fff00713          	li	a4,-1
   10afc:	03771713          	slli	a4,a4,0x37
   10b00:	fff70713          	addi	a4,a4,-1
   10b04:	00e7f7b3          	and	a5,a5,a4
   10b08:	7ff00713          	li	a4,2047
   10b0c:	0037d793          	srli	a5,a5,0x3
   10b10:	00e41a63          	bne	s0,a4,10b24 <__subdf3+0x428>
   10b14:	00078863          	beqz	a5,10b24 <__subdf3+0x428>
   10b18:	00100793          	li	a5,1
   10b1c:	03379793          	slli	a5,a5,0x33
   10b20:	00000913          	li	s2,0
   10b24:	7ff47513          	andi	a0,s0,2047
   10b28:	01813083          	ld	ra,24(sp)
   10b2c:	01013403          	ld	s0,16(sp)
   10b30:	00c79793          	slli	a5,a5,0xc
   10b34:	03451513          	slli	a0,a0,0x34
   10b38:	00c7d793          	srli	a5,a5,0xc
   10b3c:	03f91913          	slli	s2,s2,0x3f
   10b40:	00a7e7b3          	or	a5,a5,a0
   10b44:	0127e533          	or	a0,a5,s2
   10b48:	00813483          	ld	s1,8(sp)
   10b4c:	00013903          	ld	s2,0(sp)
   10b50:	02010113          	addi	sp,sp,32
   10b54:	00008067          	ret
   10b58:	00080793          	mv	a5,a6
   10b5c:	00050413          	mv	s0,a0
   10b60:	ea9ff06f          	j	10a08 <__subdf3+0x30c>
   10b64:	00000793          	li	a5,0
   10b68:	fa1ff06f          	j	10b08 <__subdf3+0x40c>
   10b6c:	fff74713          	not	a4,a4
   10b70:	0007071b          	sext.w	a4,a4
   10b74:	ce5ff06f          	j	10858 <__subdf3+0x15c>
   10b78:	fff74713          	not	a4,a4
   10b7c:	0007071b          	sext.w	a4,a4
   10b80:	e3dff06f          	j	109bc <__subdf3+0x2c0>

0000000000010b84 <__addsf3>:
   10b84:	00800737          	lui	a4,0x800
   10b88:	fe010113          	addi	sp,sp,-32
   10b8c:	fff70713          	addi	a4,a4,-1 # 7fffff <_end+0x7ec4af>
   10b90:	00a777b3          	and	a5,a4,a0
   10b94:	00813823          	sd	s0,16(sp)
   10b98:	00b77733          	and	a4,a4,a1
   10b9c:	0175541b          	srliw	s0,a0,0x17
   10ba0:	01213023          	sd	s2,0(sp)
   10ba4:	01f5591b          	srliw	s2,a0,0x1f
   10ba8:	0175d51b          	srliw	a0,a1,0x17
   10bac:	02079793          	slli	a5,a5,0x20
   10bb0:	0ff47413          	andi	s0,s0,255
   10bb4:	02071713          	slli	a4,a4,0x20
   10bb8:	0ff57513          	andi	a0,a0,255
   10bbc:	0207d793          	srli	a5,a5,0x20
   10bc0:	02075713          	srli	a4,a4,0x20
   10bc4:	40a406bb          	subw	a3,s0,a0
   10bc8:	00113c23          	sd	ra,24(sp)
   10bcc:	00913423          	sd	s1,8(sp)
   10bd0:	01f5d59b          	srliw	a1,a1,0x1f
   10bd4:	00379793          	slli	a5,a5,0x3
   10bd8:	00371713          	slli	a4,a4,0x3
   10bdc:	0006861b          	sext.w	a2,a3
   10be0:	18b91463          	bne	s2,a1,10d68 <__addsf3+0x1e4>
   10be4:	08c05c63          	blez	a2,10c7c <__addsf3+0xf8>
   10be8:	04051c63          	bnez	a0,10c40 <__addsf3+0xbc>
   10bec:	34070663          	beqz	a4,10f38 <__addsf3+0x3b4>
   10bf0:	fff6861b          	addiw	a2,a3,-1
   10bf4:	02061e63          	bnez	a2,10c30 <__addsf3+0xac>
   10bf8:	00e787b3          	add	a5,a5,a4
   10bfc:	00040513          	mv	a0,s0
   10c00:	02579713          	slli	a4,a5,0x25
   10c04:	10075c63          	bgez	a4,10d1c <__addsf3+0x198>
   10c08:	00150513          	addi	a0,a0,1
   10c0c:	0ff00713          	li	a4,255
   10c10:	32e50e63          	beq	a0,a4,10f4c <__addsf3+0x3c8>
   10c14:	fc000737          	lui	a4,0xfc000
   10c18:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   10c1c:	0017f693          	andi	a3,a5,1
   10c20:	00f777b3          	and	a5,a4,a5
   10c24:	0017d793          	srli	a5,a5,0x1
   10c28:	00d7e7b3          	or	a5,a5,a3
   10c2c:	0f00006f          	j	10d1c <__addsf3+0x198>
   10c30:	0ff00693          	li	a3,255
   10c34:	00d41e63          	bne	s0,a3,10c50 <__addsf3+0xcc>
   10c38:	0ff00513          	li	a0,255
   10c3c:	0e00006f          	j	10d1c <__addsf3+0x198>
   10c40:	0ff00693          	li	a3,255
   10c44:	fed40ae3          	beq	s0,a3,10c38 <__addsf3+0xb4>
   10c48:	040006b7          	lui	a3,0x4000
   10c4c:	00d76733          	or	a4,a4,a3
   10c50:	01b00693          	li	a3,27
   10c54:	00c6d663          	ble	a2,a3,10c60 <__addsf3+0xdc>
   10c58:	00100713          	li	a4,1
   10c5c:	f9dff06f          	j	10bf8 <__addsf3+0x74>
   10c60:	04000693          	li	a3,64
   10c64:	00c755b3          	srl	a1,a4,a2
   10c68:	40c6863b          	subw	a2,a3,a2
   10c6c:	00c71733          	sll	a4,a4,a2
   10c70:	00e03733          	snez	a4,a4
   10c74:	00e5e733          	or	a4,a1,a4
   10c78:	f81ff06f          	j	10bf8 <__addsf3+0x74>
   10c7c:	06060663          	beqz	a2,10ce8 <__addsf3+0x164>
   10c80:	02041463          	bnez	s0,10ca8 <__addsf3+0x124>
   10c84:	00078e63          	beqz	a5,10ca0 <__addsf3+0x11c>
   10c88:	fff00593          	li	a1,-1
   10c8c:	00b61663          	bne	a2,a1,10c98 <__addsf3+0x114>
   10c90:	00e787b3          	add	a5,a5,a4
   10c94:	f6dff06f          	j	10c00 <__addsf3+0x7c>
   10c98:	0ff00613          	li	a2,255
   10c9c:	32c51463          	bne	a0,a2,10fc4 <__addsf3+0x440>
   10ca0:	00070793          	mv	a5,a4
   10ca4:	0780006f          	j	10d1c <__addsf3+0x198>
   10ca8:	0ff00613          	li	a2,255
   10cac:	fec50ae3          	beq	a0,a2,10ca0 <__addsf3+0x11c>
   10cb0:	04000637          	lui	a2,0x4000
   10cb4:	40d006bb          	negw	a3,a3
   10cb8:	00c7e7b3          	or	a5,a5,a2
   10cbc:	01b00613          	li	a2,27
   10cc0:	00d65663          	ble	a3,a2,10ccc <__addsf3+0x148>
   10cc4:	00100793          	li	a5,1
   10cc8:	fc9ff06f          	j	10c90 <__addsf3+0x10c>
   10ccc:	04000613          	li	a2,64
   10cd0:	00d7d5b3          	srl	a1,a5,a3
   10cd4:	40d606bb          	subw	a3,a2,a3
   10cd8:	00d797b3          	sll	a5,a5,a3
   10cdc:	00f037b3          	snez	a5,a5
   10ce0:	00f5e7b3          	or	a5,a1,a5
   10ce4:	fadff06f          	j	10c90 <__addsf3+0x10c>
   10ce8:	00140693          	addi	a3,s0,1
   10cec:	0fe6f513          	andi	a0,a3,254
   10cf0:	06051063          	bnez	a0,10d50 <__addsf3+0x1cc>
   10cf4:	04041263          	bnez	s0,10d38 <__addsf3+0x1b4>
   10cf8:	fa0784e3          	beqz	a5,10ca0 <__addsf3+0x11c>
   10cfc:	02070063          	beqz	a4,10d1c <__addsf3+0x198>
   10d00:	00e787b3          	add	a5,a5,a4
   10d04:	02579713          	slli	a4,a5,0x25
   10d08:	00075a63          	bgez	a4,10d1c <__addsf3+0x198>
   10d0c:	fc000737          	lui	a4,0xfc000
   10d10:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   10d14:	00e7f7b3          	and	a5,a5,a4
   10d18:	00100513          	li	a0,1
   10d1c:	0077f713          	andi	a4,a5,7
   10d20:	22070863          	beqz	a4,10f50 <__addsf3+0x3cc>
   10d24:	00f7f713          	andi	a4,a5,15
   10d28:	00400693          	li	a3,4
   10d2c:	22d70263          	beq	a4,a3,10f50 <__addsf3+0x3cc>
   10d30:	00478793          	addi	a5,a5,4
   10d34:	21c0006f          	j	10f50 <__addsf3+0x3cc>
   10d38:	20078463          	beqz	a5,10f40 <__addsf3+0x3bc>
   10d3c:	ee070ee3          	beqz	a4,10c38 <__addsf3+0xb4>
   10d40:	00000913          	li	s2,0
   10d44:	020007b7          	lui	a5,0x2000
   10d48:	0ff00513          	li	a0,255
   10d4c:	2040006f          	j	10f50 <__addsf3+0x3cc>
   10d50:	0ff00613          	li	a2,255
   10d54:	1ec68a63          	beq	a3,a2,10f48 <__addsf3+0x3c4>
   10d58:	00e787b3          	add	a5,a5,a4
   10d5c:	0017d793          	srli	a5,a5,0x1
   10d60:	00068513          	mv	a0,a3
   10d64:	fb9ff06f          	j	10d1c <__addsf3+0x198>
   10d68:	08c05863          	blez	a2,10df8 <__addsf3+0x274>
   10d6c:	04051863          	bnez	a0,10dbc <__addsf3+0x238>
   10d70:	00040513          	mv	a0,s0
   10d74:	fa0704e3          	beqz	a4,10d1c <__addsf3+0x198>
   10d78:	fff6861b          	addiw	a2,a3,-1
   10d7c:	02061263          	bnez	a2,10da0 <__addsf3+0x21c>
   10d80:	40e787b3          	sub	a5,a5,a4
   10d84:	02579713          	slli	a4,a5,0x25
   10d88:	f8075ae3          	bgez	a4,10d1c <__addsf3+0x198>
   10d8c:	040004b7          	lui	s1,0x4000
   10d90:	fff48493          	addi	s1,s1,-1 # 3ffffff <_end+0x3fec4af>
   10d94:	0097f4b3          	and	s1,a5,s1
   10d98:	00050413          	mv	s0,a0
   10d9c:	13c0006f          	j	10ed8 <__addsf3+0x354>
   10da0:	0ff00693          	li	a3,255
   10da4:	0ff00513          	li	a0,255
   10da8:	f6d40ae3          	beq	s0,a3,10d1c <__addsf3+0x198>
   10dac:	01b00693          	li	a3,27
   10db0:	02c6d263          	ble	a2,a3,10dd4 <__addsf3+0x250>
   10db4:	00100713          	li	a4,1
   10db8:	0340006f          	j	10dec <__addsf3+0x268>
   10dbc:	0ff00693          	li	a3,255
   10dc0:	0ff00513          	li	a0,255
   10dc4:	f4d40ce3          	beq	s0,a3,10d1c <__addsf3+0x198>
   10dc8:	040006b7          	lui	a3,0x4000
   10dcc:	00d76733          	or	a4,a4,a3
   10dd0:	fddff06f          	j	10dac <__addsf3+0x228>
   10dd4:	04000693          	li	a3,64
   10dd8:	00c755b3          	srl	a1,a4,a2
   10ddc:	40c6863b          	subw	a2,a3,a2
   10de0:	00c71733          	sll	a4,a4,a2
   10de4:	00e03733          	snez	a4,a4
   10de8:	00e5e733          	or	a4,a1,a4
   10dec:	40e787b3          	sub	a5,a5,a4
   10df0:	00040513          	mv	a0,s0
   10df4:	f91ff06f          	j	10d84 <__addsf3+0x200>
   10df8:	06060a63          	beqz	a2,10e6c <__addsf3+0x2e8>
   10dfc:	02041863          	bnez	s0,10e2c <__addsf3+0x2a8>
   10e00:	02078063          	beqz	a5,10e20 <__addsf3+0x29c>
   10e04:	fff00813          	li	a6,-1
   10e08:	01061863          	bne	a2,a6,10e18 <__addsf3+0x294>
   10e0c:	40f707b3          	sub	a5,a4,a5
   10e10:	00058913          	mv	s2,a1
   10e14:	f71ff06f          	j	10d84 <__addsf3+0x200>
   10e18:	0ff00613          	li	a2,255
   10e1c:	1ac51a63          	bne	a0,a2,10fd0 <__addsf3+0x44c>
   10e20:	00070793          	mv	a5,a4
   10e24:	00058913          	mv	s2,a1
   10e28:	ef5ff06f          	j	10d1c <__addsf3+0x198>
   10e2c:	0ff00613          	li	a2,255
   10e30:	fec508e3          	beq	a0,a2,10e20 <__addsf3+0x29c>
   10e34:	04000637          	lui	a2,0x4000
   10e38:	40d006bb          	negw	a3,a3
   10e3c:	00c7e7b3          	or	a5,a5,a2
   10e40:	01b00613          	li	a2,27
   10e44:	00d65663          	ble	a3,a2,10e50 <__addsf3+0x2cc>
   10e48:	00100793          	li	a5,1
   10e4c:	fc1ff06f          	j	10e0c <__addsf3+0x288>
   10e50:	04000613          	li	a2,64
   10e54:	00d7d833          	srl	a6,a5,a3
   10e58:	40d606bb          	subw	a3,a2,a3
   10e5c:	00d797b3          	sll	a5,a5,a3
   10e60:	00f037b3          	snez	a5,a5
   10e64:	00f867b3          	or	a5,a6,a5
   10e68:	fa5ff06f          	j	10e0c <__addsf3+0x288>
   10e6c:	00140513          	addi	a0,s0,1
   10e70:	0fe57513          	andi	a0,a0,254
   10e74:	04051863          	bnez	a0,10ec4 <__addsf3+0x340>
   10e78:	02041c63          	bnez	s0,10eb0 <__addsf3+0x32c>
   10e7c:	00079a63          	bnez	a5,10e90 <__addsf3+0x30c>
   10e80:	fa0710e3          	bnez	a4,10e20 <__addsf3+0x29c>
   10e84:	00000793          	li	a5,0
   10e88:	00000913          	li	s2,0
   10e8c:	0c40006f          	j	10f50 <__addsf3+0x3cc>
   10e90:	e80706e3          	beqz	a4,10d1c <__addsf3+0x198>
   10e94:	40e786b3          	sub	a3,a5,a4
   10e98:	02569613          	slli	a2,a3,0x25
   10e9c:	40f707b3          	sub	a5,a4,a5
   10ea0:	f80642e3          	bltz	a2,10e24 <__addsf3+0x2a0>
   10ea4:	fe0680e3          	beqz	a3,10e84 <__addsf3+0x300>
   10ea8:	00068793          	mv	a5,a3
   10eac:	e71ff06f          	j	10d1c <__addsf3+0x198>
   10eb0:	e80796e3          	bnez	a5,10d3c <__addsf3+0x1b8>
   10eb4:	e80706e3          	beqz	a4,10d40 <__addsf3+0x1bc>
   10eb8:	00070793          	mv	a5,a4
   10ebc:	00058913          	mv	s2,a1
   10ec0:	d79ff06f          	j	10c38 <__addsf3+0xb4>
   10ec4:	40e784b3          	sub	s1,a5,a4
   10ec8:	02549693          	slli	a3,s1,0x25
   10ecc:	0406d463          	bgez	a3,10f14 <__addsf3+0x390>
   10ed0:	40f704b3          	sub	s1,a4,a5
   10ed4:	00058913          	mv	s2,a1
   10ed8:	00048513          	mv	a0,s1
   10edc:	2cd000ef          	jal	ra,119a8 <__clzdi2>
   10ee0:	fdb5051b          	addiw	a0,a0,-37
   10ee4:	00a494b3          	sll	s1,s1,a0
   10ee8:	02854e63          	blt	a0,s0,10f24 <__addsf3+0x3a0>
   10eec:	4085043b          	subw	s0,a0,s0
   10ef0:	0014041b          	addiw	s0,s0,1
   10ef4:	04000513          	li	a0,64
   10ef8:	0084d7b3          	srl	a5,s1,s0
   10efc:	4085043b          	subw	s0,a0,s0
   10f00:	008494b3          	sll	s1,s1,s0
   10f04:	009034b3          	snez	s1,s1
   10f08:	0097e7b3          	or	a5,a5,s1
   10f0c:	00000513          	li	a0,0
   10f10:	e0dff06f          	j	10d1c <__addsf3+0x198>
   10f14:	fc0492e3          	bnez	s1,10ed8 <__addsf3+0x354>
   10f18:	00000793          	li	a5,0
   10f1c:	00000513          	li	a0,0
   10f20:	f69ff06f          	j	10e88 <__addsf3+0x304>
   10f24:	fc0007b7          	lui	a5,0xfc000
   10f28:	fff78793          	addi	a5,a5,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   10f2c:	40a40533          	sub	a0,s0,a0
   10f30:	00f4f7b3          	and	a5,s1,a5
   10f34:	de9ff06f          	j	10d1c <__addsf3+0x198>
   10f38:	00040513          	mv	a0,s0
   10f3c:	de1ff06f          	j	10d1c <__addsf3+0x198>
   10f40:	00070793          	mv	a5,a4
   10f44:	cf5ff06f          	j	10c38 <__addsf3+0xb4>
   10f48:	0ff00513          	li	a0,255
   10f4c:	00000793          	li	a5,0
   10f50:	02579713          	slli	a4,a5,0x25
   10f54:	00075e63          	bgez	a4,10f70 <__addsf3+0x3ec>
   10f58:	00150513          	addi	a0,a0,1
   10f5c:	0ff00713          	li	a4,255
   10f60:	04e50e63          	beq	a0,a4,10fbc <__addsf3+0x438>
   10f64:	fc000737          	lui	a4,0xfc000
   10f68:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   10f6c:	00e7f7b3          	and	a5,a5,a4
   10f70:	0ff00713          	li	a4,255
   10f74:	0037d793          	srli	a5,a5,0x3
   10f78:	00e51863          	bne	a0,a4,10f88 <__addsf3+0x404>
   10f7c:	00078663          	beqz	a5,10f88 <__addsf3+0x404>
   10f80:	004007b7          	lui	a5,0x400
   10f84:	00000913          	li	s2,0
   10f88:	0ff57513          	andi	a0,a0,255
   10f8c:	01813083          	ld	ra,24(sp)
   10f90:	01013403          	ld	s0,16(sp)
   10f94:	02979793          	slli	a5,a5,0x29
   10f98:	0175171b          	slliw	a4,a0,0x17
   10f9c:	0297d513          	srli	a0,a5,0x29
   10fa0:	01f9191b          	slliw	s2,s2,0x1f
   10fa4:	00e56533          	or	a0,a0,a4
   10fa8:	01256533          	or	a0,a0,s2
   10fac:	00813483          	ld	s1,8(sp)
   10fb0:	00013903          	ld	s2,0(sp)
   10fb4:	02010113          	addi	sp,sp,32
   10fb8:	00008067          	ret
   10fbc:	00000793          	li	a5,0
   10fc0:	fb1ff06f          	j	10f70 <__addsf3+0x3ec>
   10fc4:	fff6c693          	not	a3,a3
   10fc8:	0006869b          	sext.w	a3,a3
   10fcc:	cf1ff06f          	j	10cbc <__addsf3+0x138>
   10fd0:	fff6c693          	not	a3,a3
   10fd4:	0006869b          	sext.w	a3,a3
   10fd8:	e69ff06f          	j	10e40 <__addsf3+0x2bc>

0000000000010fdc <__lesf2>:
   10fdc:	008007b7          	lui	a5,0x800
   10fe0:	fff78793          	addi	a5,a5,-1 # 7fffff <_end+0x7ec4af>
   10fe4:	00a7f633          	and	a2,a5,a0
   10fe8:	0175581b          	srliw	a6,a0,0x17
   10fec:	00b7f7b3          	and	a5,a5,a1
   10ff0:	0175d69b          	srliw	a3,a1,0x17
   10ff4:	02061613          	slli	a2,a2,0x20
   10ff8:	01f5571b          	srliw	a4,a0,0x1f
   10ffc:	02079793          	slli	a5,a5,0x20
   11000:	0ff87813          	andi	a6,a6,255
   11004:	0ff00513          	li	a0,255
   11008:	02065613          	srli	a2,a2,0x20
   1100c:	0207d793          	srli	a5,a5,0x20
   11010:	0ff6f693          	andi	a3,a3,255
   11014:	01f5d59b          	srliw	a1,a1,0x1f
   11018:	00a81863          	bne	a6,a0,11028 <__lesf2+0x4c>
   1101c:	00200513          	li	a0,2
   11020:	06060c63          	beqz	a2,11098 <__lesf2+0xbc>
   11024:	00008067          	ret
   11028:	00a69663          	bne	a3,a0,11034 <__lesf2+0x58>
   1102c:	00200513          	li	a0,2
   11030:	04079c63          	bnez	a5,11088 <__lesf2+0xac>
   11034:	06081463          	bnez	a6,1109c <__lesf2+0xc0>
   11038:	00069463          	bnez	a3,11040 <__lesf2+0x64>
   1103c:	04078863          	beqz	a5,1108c <__lesf2+0xb0>
   11040:	00060e63          	beqz	a2,1105c <__lesf2+0x80>
   11044:	00b71463          	bne	a4,a1,1104c <__lesf2+0x70>
   11048:	0306d263          	ble	a6,a3,1106c <__lesf2+0x90>
   1104c:	00100513          	li	a0,1
   11050:	02070c63          	beqz	a4,11088 <__lesf2+0xac>
   11054:	fff00513          	li	a0,-1
   11058:	00008067          	ret
   1105c:	fff00513          	li	a0,-1
   11060:	02058463          	beqz	a1,11088 <__lesf2+0xac>
   11064:	00100513          	li	a0,1
   11068:	00008067          	ret
   1106c:	00d85863          	ble	a3,a6,1107c <__lesf2+0xa0>
   11070:	fff00513          	li	a0,-1
   11074:	fe0718e3          	bnez	a4,11064 <__lesf2+0x88>
   11078:	0100006f          	j	11088 <__lesf2+0xac>
   1107c:	fcc7e8e3          	bltu	a5,a2,1104c <__lesf2+0x70>
   11080:	00000513          	li	a0,0
   11084:	fef666e3          	bltu	a2,a5,11070 <__lesf2+0x94>
   11088:	00008067          	ret
   1108c:	00000513          	li	a0,0
   11090:	fe060ce3          	beqz	a2,11088 <__lesf2+0xac>
   11094:	fb9ff06f          	j	1104c <__lesf2+0x70>
   11098:	f9068ae3          	beq	a3,a6,1102c <__lesf2+0x50>
   1109c:	fa0694e3          	bnez	a3,11044 <__lesf2+0x68>
   110a0:	fa0786e3          	beqz	a5,1104c <__lesf2+0x70>
   110a4:	fa1ff06f          	j	11044 <__lesf2+0x68>

00000000000110a8 <__mulsf3>:
   110a8:	fc010113          	addi	sp,sp,-64
   110ac:	03213023          	sd	s2,32(sp)
   110b0:	0175591b          	srliw	s2,a0,0x17
   110b4:	02913423          	sd	s1,40(sp)
   110b8:	01313c23          	sd	s3,24(sp)
   110bc:	01513423          	sd	s5,8(sp)
   110c0:	02951493          	slli	s1,a0,0x29
   110c4:	02113c23          	sd	ra,56(sp)
   110c8:	02813823          	sd	s0,48(sp)
   110cc:	01413823          	sd	s4,16(sp)
   110d0:	0ff97913          	andi	s2,s2,255
   110d4:	00058a93          	mv	s5,a1
   110d8:	0294d493          	srli	s1,s1,0x29
   110dc:	01f5599b          	srliw	s3,a0,0x1f
   110e0:	08090663          	beqz	s2,1116c <__mulsf3+0xc4>
   110e4:	0ff00793          	li	a5,255
   110e8:	0af90463          	beq	s2,a5,11190 <__mulsf3+0xe8>
   110ec:	00349493          	slli	s1,s1,0x3
   110f0:	040007b7          	lui	a5,0x4000
   110f4:	00f4e4b3          	or	s1,s1,a5
   110f8:	f8190913          	addi	s2,s2,-127
   110fc:	00000a13          	li	s4,0
   11100:	017ad51b          	srliw	a0,s5,0x17
   11104:	029a9413          	slli	s0,s5,0x29
   11108:	0ff57513          	andi	a0,a0,255
   1110c:	02945413          	srli	s0,s0,0x29
   11110:	01fada9b          	srliw	s5,s5,0x1f
   11114:	08050e63          	beqz	a0,111b0 <__mulsf3+0x108>
   11118:	0ff00793          	li	a5,255
   1111c:	0af50c63          	beq	a0,a5,111d4 <__mulsf3+0x12c>
   11120:	00341413          	slli	s0,s0,0x3
   11124:	040007b7          	lui	a5,0x4000
   11128:	00f46433          	or	s0,s0,a5
   1112c:	f8150513          	addi	a0,a0,-127
   11130:	00000793          	li	a5,0
   11134:	002a1713          	slli	a4,s4,0x2
   11138:	00f76733          	or	a4,a4,a5
   1113c:	00a90933          	add	s2,s2,a0
   11140:	fff70713          	addi	a4,a4,-1
   11144:	00e00613          	li	a2,14
   11148:	0159c6b3          	xor	a3,s3,s5
   1114c:	00190593          	addi	a1,s2,1
   11150:	0ae66263          	bltu	a2,a4,111f4 <__mulsf3+0x14c>
   11154:	00012637          	lui	a2,0x12
   11158:	00271713          	slli	a4,a4,0x2
   1115c:	9e860613          	addi	a2,a2,-1560 # 119e8 <__clzdi2+0x40>
   11160:	00c70733          	add	a4,a4,a2
   11164:	00072703          	lw	a4,0(a4)
   11168:	00070067          	jr	a4
   1116c:	02048c63          	beqz	s1,111a4 <__mulsf3+0xfc>
   11170:	00048513          	mv	a0,s1
   11174:	035000ef          	jal	ra,119a8 <__clzdi2>
   11178:	0005051b          	sext.w	a0,a0
   1117c:	fdb5079b          	addiw	a5,a0,-37
   11180:	faa00913          	li	s2,-86
   11184:	00f494b3          	sll	s1,s1,a5
   11188:	40a90933          	sub	s2,s2,a0
   1118c:	f71ff06f          	j	110fc <__mulsf3+0x54>
   11190:	0ff00913          	li	s2,255
   11194:	00200a13          	li	s4,2
   11198:	f60484e3          	beqz	s1,11100 <__mulsf3+0x58>
   1119c:	00300a13          	li	s4,3
   111a0:	f61ff06f          	j	11100 <__mulsf3+0x58>
   111a4:	00000913          	li	s2,0
   111a8:	00100a13          	li	s4,1
   111ac:	f55ff06f          	j	11100 <__mulsf3+0x58>
   111b0:	02040c63          	beqz	s0,111e8 <__mulsf3+0x140>
   111b4:	00040513          	mv	a0,s0
   111b8:	7f0000ef          	jal	ra,119a8 <__clzdi2>
   111bc:	0005051b          	sext.w	a0,a0
   111c0:	fdb5079b          	addiw	a5,a0,-37
   111c4:	00f41433          	sll	s0,s0,a5
   111c8:	faa00793          	li	a5,-86
   111cc:	40a78533          	sub	a0,a5,a0
   111d0:	f61ff06f          	j	11130 <__mulsf3+0x88>
   111d4:	0ff00513          	li	a0,255
   111d8:	00200793          	li	a5,2
   111dc:	f4040ce3          	beqz	s0,11134 <__mulsf3+0x8c>
   111e0:	00300793          	li	a5,3
   111e4:	f51ff06f          	j	11134 <__mulsf3+0x8c>
   111e8:	00000513          	li	a0,0
   111ec:	00100793          	li	a5,1
   111f0:	f45ff06f          	j	11134 <__mulsf3+0x8c>
   111f4:	028484b3          	mul	s1,s1,s0
   111f8:	01a4d413          	srli	s0,s1,0x1a
   111fc:	02649493          	slli	s1,s1,0x26
   11200:	009034b3          	snez	s1,s1
   11204:	00946433          	or	s0,s0,s1
   11208:	02441793          	slli	a5,s0,0x24
   1120c:	0807dc63          	bgez	a5,112a4 <__mulsf3+0x1fc>
   11210:	00145793          	srli	a5,s0,0x1
   11214:	00147413          	andi	s0,s0,1
   11218:	0087e433          	or	s0,a5,s0
   1121c:	07f58793          	addi	a5,a1,127
   11220:	08f05663          	blez	a5,112ac <__mulsf3+0x204>
   11224:	00747713          	andi	a4,s0,7
   11228:	00070a63          	beqz	a4,1123c <__mulsf3+0x194>
   1122c:	00f47713          	andi	a4,s0,15
   11230:	00400613          	li	a2,4
   11234:	00c70463          	beq	a4,a2,1123c <__mulsf3+0x194>
   11238:	00440413          	addi	s0,s0,4
   1123c:	02441713          	slli	a4,s0,0x24
   11240:	00075a63          	bgez	a4,11254 <__mulsf3+0x1ac>
   11244:	f80007b7          	lui	a5,0xf8000
   11248:	fff78793          	addi	a5,a5,-1 # fffffffff7ffffff <_end+0xfffffffff7fec4af>
   1124c:	00f47433          	and	s0,s0,a5
   11250:	08058793          	addi	a5,a1,128
   11254:	0fe00713          	li	a4,254
   11258:	00345413          	srli	s0,s0,0x3
   1125c:	0af75c63          	ble	a5,a4,11314 <__mulsf3+0x26c>
   11260:	00000413          	li	s0,0
   11264:	0ff00793          	li	a5,255
   11268:	0ac0006f          	j	11314 <__mulsf3+0x26c>
   1126c:	00098693          	mv	a3,s3
   11270:	00048413          	mv	s0,s1
   11274:	000a0793          	mv	a5,s4
   11278:	00200713          	li	a4,2
   1127c:	fee782e3          	beq	a5,a4,11260 <__mulsf3+0x1b8>
   11280:	00300713          	li	a4,3
   11284:	08e78263          	beq	a5,a4,11308 <__mulsf3+0x260>
   11288:	00100713          	li	a4,1
   1128c:	f8e798e3          	bne	a5,a4,1121c <__mulsf3+0x174>
   11290:	00000413          	li	s0,0
   11294:	00000793          	li	a5,0
   11298:	07c0006f          	j	11314 <__mulsf3+0x26c>
   1129c:	000a8693          	mv	a3,s5
   112a0:	fd9ff06f          	j	11278 <__mulsf3+0x1d0>
   112a4:	00090593          	mv	a1,s2
   112a8:	f75ff06f          	j	1121c <__mulsf3+0x174>
   112ac:	00100713          	li	a4,1
   112b0:	40f707b3          	sub	a5,a4,a5
   112b4:	01b00713          	li	a4,27
   112b8:	fcf74ce3          	blt	a4,a5,11290 <__mulsf3+0x1e8>
   112bc:	0007871b          	sext.w	a4,a5
   112c0:	04000613          	li	a2,64
   112c4:	00e457b3          	srl	a5,s0,a4
   112c8:	40e6073b          	subw	a4,a2,a4
   112cc:	00e41433          	sll	s0,s0,a4
   112d0:	00803433          	snez	s0,s0
   112d4:	0087e433          	or	s0,a5,s0
   112d8:	00747793          	andi	a5,s0,7
   112dc:	00078a63          	beqz	a5,112f0 <__mulsf3+0x248>
   112e0:	00f47793          	andi	a5,s0,15
   112e4:	00400713          	li	a4,4
   112e8:	00e78463          	beq	a5,a4,112f0 <__mulsf3+0x248>
   112ec:	00440413          	addi	s0,s0,4
   112f0:	02541793          	slli	a5,s0,0x25
   112f4:	00345413          	srli	s0,s0,0x3
   112f8:	f807dee3          	bgez	a5,11294 <__mulsf3+0x1ec>
   112fc:	00000413          	li	s0,0
   11300:	00100793          	li	a5,1
   11304:	0100006f          	j	11314 <__mulsf3+0x26c>
   11308:	00400437          	lui	s0,0x400
   1130c:	0ff00793          	li	a5,255
   11310:	00000693          	li	a3,0
   11314:	02941413          	slli	s0,s0,0x29
   11318:	0ff7f793          	andi	a5,a5,255
   1131c:	0177979b          	slliw	a5,a5,0x17
   11320:	02945413          	srli	s0,s0,0x29
   11324:	00f46433          	or	s0,s0,a5
   11328:	01f6951b          	slliw	a0,a3,0x1f
   1132c:	00a46533          	or	a0,s0,a0
   11330:	03813083          	ld	ra,56(sp)
   11334:	03013403          	ld	s0,48(sp)
   11338:	02813483          	ld	s1,40(sp)
   1133c:	02013903          	ld	s2,32(sp)
   11340:	01813983          	ld	s3,24(sp)
   11344:	01013a03          	ld	s4,16(sp)
   11348:	00813a83          	ld	s5,8(sp)
   1134c:	04010113          	addi	sp,sp,64
   11350:	00008067          	ret

0000000000011354 <__subsf3>:
   11354:	00800737          	lui	a4,0x800
   11358:	fff70713          	addi	a4,a4,-1 # 7fffff <_end+0x7ec4af>
   1135c:	00a777b3          	and	a5,a4,a0
   11360:	00b77733          	and	a4,a4,a1
   11364:	fe010113          	addi	sp,sp,-32
   11368:	0175561b          	srliw	a2,a0,0x17
   1136c:	0175d89b          	srliw	a7,a1,0x17
   11370:	02079793          	slli	a5,a5,0x20
   11374:	02071713          	slli	a4,a4,0x20
   11378:	00913423          	sd	s1,8(sp)
   1137c:	01213023          	sd	s2,0(sp)
   11380:	0207d793          	srli	a5,a5,0x20
   11384:	0ff67613          	andi	a2,a2,255
   11388:	02075713          	srli	a4,a4,0x20
   1138c:	0ff8f893          	andi	a7,a7,255
   11390:	00113c23          	sd	ra,24(sp)
   11394:	00813823          	sd	s0,16(sp)
   11398:	0ff00693          	li	a3,255
   1139c:	01f5549b          	srliw	s1,a0,0x1f
   113a0:	00060913          	mv	s2,a2
   113a4:	00379793          	slli	a5,a5,0x3
   113a8:	00088513          	mv	a0,a7
   113ac:	01f5d59b          	srliw	a1,a1,0x1f
   113b0:	00371713          	slli	a4,a4,0x3
   113b4:	00d89463          	bne	a7,a3,113bc <__subsf3+0x68>
   113b8:	00071463          	bnez	a4,113c0 <__subsf3+0x6c>
   113bc:	0015c593          	xori	a1,a1,1
   113c0:	411606bb          	subw	a3,a2,a7
   113c4:	0006881b          	sext.w	a6,a3
   113c8:	16959e63          	bne	a1,s1,11544 <__subsf3+0x1f0>
   113cc:	09005c63          	blez	a6,11464 <__subsf3+0x110>
   113d0:	04089c63          	bnez	a7,11428 <__subsf3+0xd4>
   113d4:	34070663          	beqz	a4,11720 <__subsf3+0x3cc>
   113d8:	fff6881b          	addiw	a6,a3,-1
   113dc:	02081e63          	bnez	a6,11418 <__subsf3+0xc4>
   113e0:	00e787b3          	add	a5,a5,a4
   113e4:	00060513          	mv	a0,a2
   113e8:	02579713          	slli	a4,a5,0x25
   113ec:	10075c63          	bgez	a4,11504 <__subsf3+0x1b0>
   113f0:	00150513          	addi	a0,a0,1
   113f4:	0ff00713          	li	a4,255
   113f8:	32e50e63          	beq	a0,a4,11734 <__subsf3+0x3e0>
   113fc:	fc000737          	lui	a4,0xfc000
   11400:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   11404:	0017f693          	andi	a3,a5,1
   11408:	00f777b3          	and	a5,a4,a5
   1140c:	0017d793          	srli	a5,a5,0x1
   11410:	00d7e7b3          	or	a5,a5,a3
   11414:	0f00006f          	j	11504 <__subsf3+0x1b0>
   11418:	0ff00693          	li	a3,255
   1141c:	00d61e63          	bne	a2,a3,11438 <__subsf3+0xe4>
   11420:	0ff00513          	li	a0,255
   11424:	0e00006f          	j	11504 <__subsf3+0x1b0>
   11428:	0ff00693          	li	a3,255
   1142c:	fed60ae3          	beq	a2,a3,11420 <__subsf3+0xcc>
   11430:	040006b7          	lui	a3,0x4000
   11434:	00d76733          	or	a4,a4,a3
   11438:	01b00693          	li	a3,27
   1143c:	0106d663          	ble	a6,a3,11448 <__subsf3+0xf4>
   11440:	00100713          	li	a4,1
   11444:	f9dff06f          	j	113e0 <__subsf3+0x8c>
   11448:	04000693          	li	a3,64
   1144c:	010755b3          	srl	a1,a4,a6
   11450:	4106883b          	subw	a6,a3,a6
   11454:	01071733          	sll	a4,a4,a6
   11458:	00e03733          	snez	a4,a4
   1145c:	00e5e733          	or	a4,a1,a4
   11460:	f81ff06f          	j	113e0 <__subsf3+0x8c>
   11464:	06080663          	beqz	a6,114d0 <__subsf3+0x17c>
   11468:	02061463          	bnez	a2,11490 <__subsf3+0x13c>
   1146c:	2a078e63          	beqz	a5,11728 <__subsf3+0x3d4>
   11470:	fff00613          	li	a2,-1
   11474:	00c81663          	bne	a6,a2,11480 <__subsf3+0x12c>
   11478:	00e787b3          	add	a5,a5,a4
   1147c:	f6dff06f          	j	113e8 <__subsf3+0x94>
   11480:	0ff00613          	li	a2,255
   11484:	32c89863          	bne	a7,a2,117b4 <__subsf3+0x460>
   11488:	00070793          	mv	a5,a4
   1148c:	f95ff06f          	j	11420 <__subsf3+0xcc>
   11490:	0ff00613          	li	a2,255
   11494:	fec88ae3          	beq	a7,a2,11488 <__subsf3+0x134>
   11498:	04000637          	lui	a2,0x4000
   1149c:	40d006bb          	negw	a3,a3
   114a0:	00c7e7b3          	or	a5,a5,a2
   114a4:	01b00613          	li	a2,27
   114a8:	00d65663          	ble	a3,a2,114b4 <__subsf3+0x160>
   114ac:	00100793          	li	a5,1
   114b0:	fc9ff06f          	j	11478 <__subsf3+0x124>
   114b4:	04000613          	li	a2,64
   114b8:	00d7d5b3          	srl	a1,a5,a3
   114bc:	40d606bb          	subw	a3,a2,a3
   114c0:	00d797b3          	sll	a5,a5,a3
   114c4:	00f037b3          	snez	a5,a5
   114c8:	00f5e7b3          	or	a5,a1,a5
   114cc:	fadff06f          	j	11478 <__subsf3+0x124>
   114d0:	00160693          	addi	a3,a2,1 # 4000001 <_end+0x3fec4b1>
   114d4:	0fe6f513          	andi	a0,a3,254
   114d8:	04051a63          	bnez	a0,1152c <__subsf3+0x1d8>
   114dc:	04061263          	bnez	a2,11520 <__subsf3+0x1cc>
   114e0:	24078463          	beqz	a5,11728 <__subsf3+0x3d4>
   114e4:	02070063          	beqz	a4,11504 <__subsf3+0x1b0>
   114e8:	00e787b3          	add	a5,a5,a4
   114ec:	02579713          	slli	a4,a5,0x25
   114f0:	00075a63          	bgez	a4,11504 <__subsf3+0x1b0>
   114f4:	fc000737          	lui	a4,0xfc000
   114f8:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   114fc:	00e7f7b3          	and	a5,a5,a4
   11500:	00100513          	li	a0,1
   11504:	0077f713          	andi	a4,a5,7
   11508:	22070863          	beqz	a4,11738 <__subsf3+0x3e4>
   1150c:	00f7f713          	andi	a4,a5,15
   11510:	00400693          	li	a3,4
   11514:	22d70263          	beq	a4,a3,11738 <__subsf3+0x3e4>
   11518:	00478793          	addi	a5,a5,4
   1151c:	21c0006f          	j	11738 <__subsf3+0x3e4>
   11520:	f60784e3          	beqz	a5,11488 <__subsf3+0x134>
   11524:	ee070ee3          	beqz	a4,11420 <__subsf3+0xcc>
   11528:	1740006f          	j	1169c <__subsf3+0x348>
   1152c:	0ff00613          	li	a2,255
   11530:	20c68063          	beq	a3,a2,11730 <__subsf3+0x3dc>
   11534:	00e787b3          	add	a5,a5,a4
   11538:	0017d793          	srli	a5,a5,0x1
   1153c:	00068513          	mv	a0,a3
   11540:	fc5ff06f          	j	11504 <__subsf3+0x1b0>
   11544:	09005863          	blez	a6,115d4 <__subsf3+0x280>
   11548:	04089863          	bnez	a7,11598 <__subsf3+0x244>
   1154c:	00060513          	mv	a0,a2
   11550:	fa070ae3          	beqz	a4,11504 <__subsf3+0x1b0>
   11554:	fff6881b          	addiw	a6,a3,-1
   11558:	02081263          	bnez	a6,1157c <__subsf3+0x228>
   1155c:	40e787b3          	sub	a5,a5,a4
   11560:	02579713          	slli	a4,a5,0x25
   11564:	fa0750e3          	bgez	a4,11504 <__subsf3+0x1b0>
   11568:	04000437          	lui	s0,0x4000
   1156c:	fff40413          	addi	s0,s0,-1 # 3ffffff <_end+0x3fec4af>
   11570:	0087f433          	and	s0,a5,s0
   11574:	00050913          	mv	s2,a0
   11578:	1480006f          	j	116c0 <__subsf3+0x36c>
   1157c:	0ff00693          	li	a3,255
   11580:	0ff00513          	li	a0,255
   11584:	f8d600e3          	beq	a2,a3,11504 <__subsf3+0x1b0>
   11588:	01b00693          	li	a3,27
   1158c:	0306d263          	ble	a6,a3,115b0 <__subsf3+0x25c>
   11590:	00100713          	li	a4,1
   11594:	0340006f          	j	115c8 <__subsf3+0x274>
   11598:	0ff00693          	li	a3,255
   1159c:	0ff00513          	li	a0,255
   115a0:	f6d602e3          	beq	a2,a3,11504 <__subsf3+0x1b0>
   115a4:	040006b7          	lui	a3,0x4000
   115a8:	00d76733          	or	a4,a4,a3
   115ac:	fddff06f          	j	11588 <__subsf3+0x234>
   115b0:	04000693          	li	a3,64
   115b4:	010755b3          	srl	a1,a4,a6
   115b8:	4106883b          	subw	a6,a3,a6
   115bc:	01071733          	sll	a4,a4,a6
   115c0:	00e03733          	snez	a4,a4
   115c4:	00e5e733          	or	a4,a1,a4
   115c8:	40e787b3          	sub	a5,a5,a4
   115cc:	00060513          	mv	a0,a2
   115d0:	f91ff06f          	j	11560 <__subsf3+0x20c>
   115d4:	06080a63          	beqz	a6,11648 <__subsf3+0x2f4>
   115d8:	02061863          	bnez	a2,11608 <__subsf3+0x2b4>
   115dc:	08078263          	beqz	a5,11660 <__subsf3+0x30c>
   115e0:	fff00613          	li	a2,-1
   115e4:	00c81863          	bne	a6,a2,115f4 <__subsf3+0x2a0>
   115e8:	40f707b3          	sub	a5,a4,a5
   115ec:	00058493          	mv	s1,a1
   115f0:	f71ff06f          	j	11560 <__subsf3+0x20c>
   115f4:	0ff00613          	li	a2,255
   115f8:	1cc89463          	bne	a7,a2,117c0 <__subsf3+0x46c>
   115fc:	00070793          	mv	a5,a4
   11600:	00058493          	mv	s1,a1
   11604:	e1dff06f          	j	11420 <__subsf3+0xcc>
   11608:	0ff00613          	li	a2,255
   1160c:	fec888e3          	beq	a7,a2,115fc <__subsf3+0x2a8>
   11610:	04000637          	lui	a2,0x4000
   11614:	40d006bb          	negw	a3,a3
   11618:	00c7e7b3          	or	a5,a5,a2
   1161c:	01b00613          	li	a2,27
   11620:	00d65663          	ble	a3,a2,1162c <__subsf3+0x2d8>
   11624:	00100793          	li	a5,1
   11628:	fc1ff06f          	j	115e8 <__subsf3+0x294>
   1162c:	04000613          	li	a2,64
   11630:	00d7d833          	srl	a6,a5,a3
   11634:	40d606bb          	subw	a3,a2,a3
   11638:	00d797b3          	sll	a5,a5,a3
   1163c:	00f037b3          	snez	a5,a5
   11640:	00f867b3          	or	a5,a6,a5
   11644:	fa5ff06f          	j	115e8 <__subsf3+0x294>
   11648:	00160513          	addi	a0,a2,1 # 4000001 <_end+0x3fec4b1>
   1164c:	0fe57513          	andi	a0,a0,254
   11650:	04051e63          	bnez	a0,116ac <__subsf3+0x358>
   11654:	04061063          	bnez	a2,11694 <__subsf3+0x340>
   11658:	00079863          	bnez	a5,11668 <__subsf3+0x314>
   1165c:	02070663          	beqz	a4,11688 <__subsf3+0x334>
   11660:	00070793          	mv	a5,a4
   11664:	0180006f          	j	1167c <__subsf3+0x328>
   11668:	e8070ee3          	beqz	a4,11504 <__subsf3+0x1b0>
   1166c:	40e786b3          	sub	a3,a5,a4
   11670:	02569613          	slli	a2,a3,0x25
   11674:	00065863          	bgez	a2,11684 <__subsf3+0x330>
   11678:	40f707b3          	sub	a5,a4,a5
   1167c:	00058493          	mv	s1,a1
   11680:	e85ff06f          	j	11504 <__subsf3+0x1b0>
   11684:	12069063          	bnez	a3,117a4 <__subsf3+0x450>
   11688:	00000793          	li	a5,0
   1168c:	00000493          	li	s1,0
   11690:	0a80006f          	j	11738 <__subsf3+0x3e4>
   11694:	e80798e3          	bnez	a5,11524 <__subsf3+0x1d0>
   11698:	f60712e3          	bnez	a4,115fc <__subsf3+0x2a8>
   1169c:	00000493          	li	s1,0
   116a0:	020007b7          	lui	a5,0x2000
   116a4:	0ff00513          	li	a0,255
   116a8:	0900006f          	j	11738 <__subsf3+0x3e4>
   116ac:	40e78433          	sub	s0,a5,a4
   116b0:	02541693          	slli	a3,s0,0x25
   116b4:	0406d463          	bgez	a3,116fc <__subsf3+0x3a8>
   116b8:	40f70433          	sub	s0,a4,a5
   116bc:	00058493          	mv	s1,a1
   116c0:	00040513          	mv	a0,s0
   116c4:	2e4000ef          	jal	ra,119a8 <__clzdi2>
   116c8:	fdb5079b          	addiw	a5,a0,-37
   116cc:	00f41433          	sll	s0,s0,a5
   116d0:	0327ce63          	blt	a5,s2,1170c <__subsf3+0x3b8>
   116d4:	4127853b          	subw	a0,a5,s2
   116d8:	0015051b          	addiw	a0,a0,1
   116dc:	04000713          	li	a4,64
   116e0:	00a457b3          	srl	a5,s0,a0
   116e4:	40a7053b          	subw	a0,a4,a0
   116e8:	00a41433          	sll	s0,s0,a0
   116ec:	00803433          	snez	s0,s0
   116f0:	0087e7b3          	or	a5,a5,s0
   116f4:	00000513          	li	a0,0
   116f8:	e0dff06f          	j	11504 <__subsf3+0x1b0>
   116fc:	fc0412e3          	bnez	s0,116c0 <__subsf3+0x36c>
   11700:	00000793          	li	a5,0
   11704:	00000513          	li	a0,0
   11708:	f85ff06f          	j	1168c <__subsf3+0x338>
   1170c:	40f90533          	sub	a0,s2,a5
   11710:	fc0007b7          	lui	a5,0xfc000
   11714:	fff78793          	addi	a5,a5,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   11718:	00f477b3          	and	a5,s0,a5
   1171c:	de9ff06f          	j	11504 <__subsf3+0x1b0>
   11720:	00060513          	mv	a0,a2
   11724:	de1ff06f          	j	11504 <__subsf3+0x1b0>
   11728:	00070793          	mv	a5,a4
   1172c:	dd9ff06f          	j	11504 <__subsf3+0x1b0>
   11730:	0ff00513          	li	a0,255
   11734:	00000793          	li	a5,0
   11738:	02579713          	slli	a4,a5,0x25
   1173c:	00075e63          	bgez	a4,11758 <__subsf3+0x404>
   11740:	00150513          	addi	a0,a0,1
   11744:	0ff00713          	li	a4,255
   11748:	06e50263          	beq	a0,a4,117ac <__subsf3+0x458>
   1174c:	fc000737          	lui	a4,0xfc000
   11750:	fff70713          	addi	a4,a4,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   11754:	00e7f7b3          	and	a5,a5,a4
   11758:	0ff00713          	li	a4,255
   1175c:	0037d793          	srli	a5,a5,0x3
   11760:	00e51863          	bne	a0,a4,11770 <__subsf3+0x41c>
   11764:	00078663          	beqz	a5,11770 <__subsf3+0x41c>
   11768:	004007b7          	lui	a5,0x400
   1176c:	00000493          	li	s1,0
   11770:	0ff57513          	andi	a0,a0,255
   11774:	01813083          	ld	ra,24(sp)
   11778:	01013403          	ld	s0,16(sp)
   1177c:	02979793          	slli	a5,a5,0x29
   11780:	0175171b          	slliw	a4,a0,0x17
   11784:	0297d513          	srli	a0,a5,0x29
   11788:	01f4949b          	slliw	s1,s1,0x1f
   1178c:	00e56533          	or	a0,a0,a4
   11790:	00956533          	or	a0,a0,s1
   11794:	00013903          	ld	s2,0(sp)
   11798:	00813483          	ld	s1,8(sp)
   1179c:	02010113          	addi	sp,sp,32
   117a0:	00008067          	ret
   117a4:	00068793          	mv	a5,a3
   117a8:	d5dff06f          	j	11504 <__subsf3+0x1b0>
   117ac:	00000793          	li	a5,0
   117b0:	fa9ff06f          	j	11758 <__subsf3+0x404>
   117b4:	fff6c693          	not	a3,a3
   117b8:	0006869b          	sext.w	a3,a3
   117bc:	ce9ff06f          	j	114a4 <__subsf3+0x150>
   117c0:	fff6c693          	not	a3,a3
   117c4:	0006869b          	sext.w	a3,a3
   117c8:	e55ff06f          	j	1161c <__subsf3+0x2c8>

00000000000117cc <__extendsfdf2>:
   117cc:	0175579b          	srliw	a5,a0,0x17
   117d0:	fe010113          	addi	sp,sp,-32
   117d4:	0ff7f793          	andi	a5,a5,255
   117d8:	00813823          	sd	s0,16(sp)
   117dc:	00913423          	sd	s1,8(sp)
   117e0:	02951413          	slli	s0,a0,0x29
   117e4:	01f5549b          	srliw	s1,a0,0x1f
   117e8:	00178513          	addi	a0,a5,1 # 400001 <_end+0x3ec4b1>
   117ec:	00113c23          	sd	ra,24(sp)
   117f0:	0fe57513          	andi	a0,a0,254
   117f4:	02945413          	srli	s0,s0,0x29
   117f8:	02050e63          	beqz	a0,11834 <__extendsfdf2+0x68>
   117fc:	38078513          	addi	a0,a5,896
   11800:	01d41413          	slli	s0,s0,0x1d
   11804:	7ff57513          	andi	a0,a0,2047
   11808:	00c41413          	slli	s0,s0,0xc
   1180c:	03451793          	slli	a5,a0,0x34
   11810:	00c45413          	srli	s0,s0,0xc
   11814:	03f49513          	slli	a0,s1,0x3f
   11818:	00f46433          	or	s0,s0,a5
   1181c:	00a46533          	or	a0,s0,a0
   11820:	01813083          	ld	ra,24(sp)
   11824:	01013403          	ld	s0,16(sp)
   11828:	00813483          	ld	s1,8(sp)
   1182c:	02010113          	addi	sp,sp,32
   11830:	00008067          	ret
   11834:	02079263          	bnez	a5,11858 <__extendsfdf2+0x8c>
   11838:	fc0406e3          	beqz	s0,11804 <__extendsfdf2+0x38>
   1183c:	00040513          	mv	a0,s0
   11840:	168000ef          	jal	ra,119a8 <__clzdi2>
   11844:	ff55079b          	addiw	a5,a0,-11
   11848:	00f41433          	sll	s0,s0,a5
   1184c:	3a900793          	li	a5,937
   11850:	40a7853b          	subw	a0,a5,a0
   11854:	fb1ff06f          	j	11804 <__extendsfdf2+0x38>
   11858:	7ff00513          	li	a0,2047
   1185c:	fa0404e3          	beqz	s0,11804 <__extendsfdf2+0x38>
   11860:	00100793          	li	a5,1
   11864:	01d41413          	slli	s0,s0,0x1d
   11868:	03379793          	slli	a5,a5,0x33
   1186c:	00f46433          	or	s0,s0,a5
   11870:	f95ff06f          	j	11804 <__extendsfdf2+0x38>

0000000000011874 <__truncdfsf2>:
   11874:	03455713          	srli	a4,a0,0x34
   11878:	7ff77713          	andi	a4,a4,2047
   1187c:	00c51693          	slli	a3,a0,0xc
   11880:	00170613          	addi	a2,a4,1
   11884:	00c6d693          	srli	a3,a3,0xc
   11888:	7fe67613          	andi	a2,a2,2046
   1188c:	03f55513          	srli	a0,a0,0x3f
   11890:	00369793          	slli	a5,a3,0x3
   11894:	06060663          	beqz	a2,11900 <__truncdfsf2+0x8c>
   11898:	c8070713          	addi	a4,a4,-896
   1189c:	0fe00613          	li	a2,254
   118a0:	08e64263          	blt	a2,a4,11924 <__truncdfsf2+0xb0>
   118a4:	04e04463          	bgtz	a4,118ec <__truncdfsf2+0x78>
   118a8:	fe900693          	li	a3,-23
   118ac:	0cd74c63          	blt	a4,a3,11984 <__truncdfsf2+0x110>
   118b0:	00100693          	li	a3,1
   118b4:	03769693          	slli	a3,a3,0x37
   118b8:	00d7e7b3          	or	a5,a5,a3
   118bc:	0007071b          	sext.w	a4,a4
   118c0:	01e00693          	li	a3,30
   118c4:	40e686bb          	subw	a3,a3,a4
   118c8:	0227071b          	addiw	a4,a4,34
   118cc:	00d7d6b3          	srl	a3,a5,a3
   118d0:	00e797b3          	sll	a5,a5,a4
   118d4:	00f037b3          	snez	a5,a5
   118d8:	00f6e7b3          	or	a5,a3,a5
   118dc:	00000713          	li	a4,0
   118e0:	0077f693          	andi	a3,a5,7
   118e4:	04068463          	beqz	a3,1192c <__truncdfsf2+0xb8>
   118e8:	0a40006f          	j	1198c <__truncdfsf2+0x118>
   118ec:	02669693          	slli	a3,a3,0x26
   118f0:	01d7d793          	srli	a5,a5,0x1d
   118f4:	00d036b3          	snez	a3,a3
   118f8:	00d7e7b3          	or	a5,a5,a3
   118fc:	fe5ff06f          	j	118e0 <__truncdfsf2+0x6c>
   11900:	00071663          	bnez	a4,1190c <__truncdfsf2+0x98>
   11904:	00f037b3          	snez	a5,a5
   11908:	fd9ff06f          	j	118e0 <__truncdfsf2+0x6c>
   1190c:	0ff00713          	li	a4,255
   11910:	00078e63          	beqz	a5,1192c <__truncdfsf2+0xb8>
   11914:	01d7d793          	srli	a5,a5,0x1d
   11918:	ff87f793          	andi	a5,a5,-8
   1191c:	020006b7          	lui	a3,0x2000
   11920:	fd9ff06f          	j	118f8 <__truncdfsf2+0x84>
   11924:	00000793          	li	a5,0
   11928:	0ff00713          	li	a4,255
   1192c:	02579693          	slli	a3,a5,0x25
   11930:	0006de63          	bgez	a3,1194c <__truncdfsf2+0xd8>
   11934:	00170713          	addi	a4,a4,1
   11938:	0ff00693          	li	a3,255
   1193c:	06d70263          	beq	a4,a3,119a0 <__truncdfsf2+0x12c>
   11940:	fc0006b7          	lui	a3,0xfc000
   11944:	fff68693          	addi	a3,a3,-1 # fffffffffbffffff <_end+0xfffffffffbfec4af>
   11948:	00d7f7b3          	and	a5,a5,a3
   1194c:	0ff00693          	li	a3,255
   11950:	0037d793          	srli	a5,a5,0x3
   11954:	00d71863          	bne	a4,a3,11964 <__truncdfsf2+0xf0>
   11958:	00078663          	beqz	a5,11964 <__truncdfsf2+0xf0>
   1195c:	004007b7          	lui	a5,0x400
   11960:	00000513          	li	a0,0
   11964:	02979793          	slli	a5,a5,0x29
   11968:	0ff77713          	andi	a4,a4,255
   1196c:	0177171b          	slliw	a4,a4,0x17
   11970:	0297d793          	srli	a5,a5,0x29
   11974:	00e7e7b3          	or	a5,a5,a4
   11978:	01f5151b          	slliw	a0,a0,0x1f
   1197c:	00a7e533          	or	a0,a5,a0
   11980:	00008067          	ret
   11984:	00100793          	li	a5,1
   11988:	00000713          	li	a4,0
   1198c:	00f7f693          	andi	a3,a5,15
   11990:	00400613          	li	a2,4
   11994:	f8c68ce3          	beq	a3,a2,1192c <__truncdfsf2+0xb8>
   11998:	00478793          	addi	a5,a5,4 # 400004 <_end+0x3ec4b4>
   1199c:	f91ff06f          	j	1192c <__truncdfsf2+0xb8>
   119a0:	00000793          	li	a5,0
   119a4:	fa9ff06f          	j	1194c <__truncdfsf2+0xd8>

00000000000119a8 <__clzdi2>:
   119a8:	03800793          	li	a5,56
   119ac:	00f55733          	srl	a4,a0,a5
   119b0:	0ff77713          	andi	a4,a4,255
   119b4:	00071663          	bnez	a4,119c0 <__clzdi2+0x18>
   119b8:	ff878793          	addi	a5,a5,-8
   119bc:	fe0798e3          	bnez	a5,119ac <__clzdi2+0x4>
   119c0:	04000713          	li	a4,64
   119c4:	40f70733          	sub	a4,a4,a5
   119c8:	00f557b3          	srl	a5,a0,a5
   119cc:	00012537          	lui	a0,0x12
   119d0:	a2850513          	addi	a0,a0,-1496 # 11a28 <__clz_tab>
   119d4:	00a787b3          	add	a5,a5,a0
   119d8:	0007c503          	lbu	a0,0(a5)
   119dc:	40a7053b          	subw	a0,a4,a0
   119e0:	00008067          	ret
