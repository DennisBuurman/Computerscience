
brainfuck.bin:     file format elf64-littleriscv


Disassembly of section .text:

00000000000100b0 <putchar>:
   100b0:	fd010113          	addi	sp,sp,-48
   100b4:	02813423          	sd	s0,40(sp)
   100b8:	03010413          	addi	s0,sp,48
   100bc:	00050793          	mv	a5,a0
   100c0:	fcf42e23          	sw	a5,-36(s0)
   100c4:	20000793          	li	a5,512
   100c8:	fef43423          	sd	a5,-24(s0)
   100cc:	fdc42783          	lw	a5,-36(s0)
   100d0:	0ff7f713          	andi	a4,a5,255
   100d4:	fe843783          	ld	a5,-24(s0)
   100d8:	00e78023          	sb	a4,0(a5)
   100dc:	00000793          	li	a5,0
   100e0:	00078513          	mv	a0,a5
   100e4:	02813403          	ld	s0,40(sp)
   100e8:	03010113          	addi	sp,sp,48
   100ec:	00008067          	ret

00000000000100f0 <puts>:
   100f0:	fd010113          	addi	sp,sp,-48
   100f4:	02113423          	sd	ra,40(sp)
   100f8:	02813023          	sd	s0,32(sp)
   100fc:	03010413          	addi	s0,sp,48
   10100:	fca43c23          	sd	a0,-40(s0)
   10104:	fd843783          	ld	a5,-40(s0)
   10108:	fef43423          	sd	a5,-24(s0)
   1010c:	0240006f          	j	10130 <puts+0x40>
   10110:	fe843783          	ld	a5,-24(s0)
   10114:	0007c783          	lbu	a5,0(a5)
   10118:	0007879b          	sext.w	a5,a5
   1011c:	00078513          	mv	a0,a5
   10120:	f91ff0ef          	jal	ra,100b0 <putchar>
   10124:	fe843783          	ld	a5,-24(s0)
   10128:	00178793          	addi	a5,a5,1
   1012c:	fef43423          	sd	a5,-24(s0)
   10130:	fe843783          	ld	a5,-24(s0)
   10134:	0007c783          	lbu	a5,0(a5)
   10138:	fc079ce3          	bnez	a5,10110 <puts+0x20>
   1013c:	00a00513          	li	a0,10
   10140:	f71ff0ef          	jal	ra,100b0 <putchar>
   10144:	00000013          	nop
   10148:	02813083          	ld	ra,40(sp)
   1014c:	02013403          	ld	s0,32(sp)
   10150:	03010113          	addi	sp,sp,48
   10154:	00008067          	ret

0000000000010158 <strlen>:
   10158:	fd010113          	addi	sp,sp,-48
   1015c:	02813423          	sd	s0,40(sp)
   10160:	03010413          	addi	s0,sp,48
   10164:	fca43c23          	sd	a0,-40(s0)
   10168:	fe042623          	sw	zero,-20(s0)
   1016c:	fd843783          	ld	a5,-40(s0)
   10170:	fef43023          	sd	a5,-32(s0)
   10174:	01c0006f          	j	10190 <strlen+0x38>
   10178:	fe043783          	ld	a5,-32(s0)
   1017c:	00178793          	addi	a5,a5,1
   10180:	fef43023          	sd	a5,-32(s0)
   10184:	fec42783          	lw	a5,-20(s0)
   10188:	0017879b          	addiw	a5,a5,1
   1018c:	fef42623          	sw	a5,-20(s0)
   10190:	fe043783          	ld	a5,-32(s0)
   10194:	0007c783          	lbu	a5,0(a5)
   10198:	fe0790e3          	bnez	a5,10178 <strlen+0x20>
   1019c:	fec42783          	lw	a5,-20(s0)
   101a0:	00078513          	mv	a0,a5
   101a4:	02813403          	ld	s0,40(sp)
   101a8:	03010113          	addi	sp,sp,48
   101ac:	00008067          	ret

00000000000101b0 <compile_bf>:
   101b0:	fd010113          	addi	sp,sp,-48
   101b4:	02813423          	sd	s0,40(sp)
   101b8:	03010413          	addi	s0,sp,48
   101bc:	fca43c23          	sd	a0,-40(s0)
   101c0:	00058793          	mv	a5,a1
   101c4:	fcf42a23          	sw	a5,-44(s0)
   101c8:	fe041723          	sh	zero,-18(s0)
   101cc:	fe042423          	sw	zero,-24(s0)
   101d0:	2800006f          	j	10450 <compile_bf+0x2a0>
   101d4:	fe842783          	lw	a5,-24(s0)
   101d8:	fd843703          	ld	a4,-40(s0)
   101dc:	00f707b3          	add	a5,a4,a5
   101e0:	0007c783          	lbu	a5,0(a5)
   101e4:	fef42223          	sw	a5,-28(s0)
   101e8:	fe442783          	lw	a5,-28(s0)
   101ec:	fd57869b          	addiw	a3,a5,-43
   101f0:	0006871b          	sext.w	a4,a3
   101f4:	03200793          	li	a5,50
   101f8:	22e7e863          	bltu	a5,a4,10428 <compile_bf+0x278>
   101fc:	02069793          	slli	a5,a3,0x20
   10200:	0207d793          	srli	a5,a5,0x20
   10204:	00279713          	slli	a4,a5,0x2
   10208:	000117b7          	lui	a5,0x11
   1020c:	88078793          	addi	a5,a5,-1920 # 10880 <_start+0xa4>
   10210:	00f707b3          	add	a5,a4,a5
   10214:	0007a783          	lw	a5,0(a5)
   10218:	00078067          	jr	a5
   1021c:	fee45783          	lhu	a5,-18(s0)
   10220:	0007871b          	sext.w	a4,a5
   10224:	000127b7          	lui	a5,0x12
   10228:	00271713          	slli	a4,a4,0x2
   1022c:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   10230:	00f707b3          	add	a5,a4,a5
   10234:	00100713          	li	a4,1
   10238:	00e79023          	sh	a4,0(a5)
   1023c:	1fc0006f          	j	10438 <compile_bf+0x288>
   10240:	fee45783          	lhu	a5,-18(s0)
   10244:	0007871b          	sext.w	a4,a5
   10248:	000127b7          	lui	a5,0x12
   1024c:	00271713          	slli	a4,a4,0x2
   10250:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   10254:	00f707b3          	add	a5,a4,a5
   10258:	00200713          	li	a4,2
   1025c:	00e79023          	sh	a4,0(a5)
   10260:	1d80006f          	j	10438 <compile_bf+0x288>
   10264:	fee45783          	lhu	a5,-18(s0)
   10268:	0007871b          	sext.w	a4,a5
   1026c:	000127b7          	lui	a5,0x12
   10270:	00271713          	slli	a4,a4,0x2
   10274:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   10278:	00f707b3          	add	a5,a4,a5
   1027c:	00300713          	li	a4,3
   10280:	00e79023          	sh	a4,0(a5)
   10284:	1b40006f          	j	10438 <compile_bf+0x288>
   10288:	fee45783          	lhu	a5,-18(s0)
   1028c:	0007871b          	sext.w	a4,a5
   10290:	000127b7          	lui	a5,0x12
   10294:	00271713          	slli	a4,a4,0x2
   10298:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   1029c:	00f707b3          	add	a5,a4,a5
   102a0:	00400713          	li	a4,4
   102a4:	00e79023          	sh	a4,0(a5)
   102a8:	1900006f          	j	10438 <compile_bf+0x288>
   102ac:	fee45783          	lhu	a5,-18(s0)
   102b0:	0007871b          	sext.w	a4,a5
   102b4:	000127b7          	lui	a5,0x12
   102b8:	00271713          	slli	a4,a4,0x2
   102bc:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   102c0:	00f707b3          	add	a5,a4,a5
   102c4:	00500713          	li	a4,5
   102c8:	00e79023          	sh	a4,0(a5)
   102cc:	16c0006f          	j	10438 <compile_bf+0x288>
   102d0:	fee45783          	lhu	a5,-18(s0)
   102d4:	0007871b          	sext.w	a4,a5
   102d8:	000127b7          	lui	a5,0x12
   102dc:	00271713          	slli	a4,a4,0x2
   102e0:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   102e4:	00f707b3          	add	a5,a4,a5
   102e8:	00600713          	li	a4,6
   102ec:	00e79023          	sh	a4,0(a5)
   102f0:	1480006f          	j	10438 <compile_bf+0x288>
   102f4:	fee45783          	lhu	a5,-18(s0)
   102f8:	0007871b          	sext.w	a4,a5
   102fc:	000127b7          	lui	a5,0x12
   10300:	00271713          	slli	a4,a4,0x2
   10304:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   10308:	00f707b3          	add	a5,a4,a5
   1030c:	00700713          	li	a4,7
   10310:	00e79023          	sh	a4,0(a5)
   10314:	000167b7          	lui	a5,0x16
   10318:	d807a783          	lw	a5,-640(a5) # 15d80 <SP>
   1031c:	00078713          	mv	a4,a5
   10320:	20000793          	li	a5,512
   10324:	00f71663          	bne	a4,a5,10330 <compile_bf+0x180>
   10328:	00100793          	li	a5,1
   1032c:	18c0006f          	j	104b8 <compile_bf+0x308>
   10330:	000167b7          	lui	a5,0x16
   10334:	d807a783          	lw	a5,-640(a5) # 15d80 <SP>
   10338:	0007879b          	sext.w	a5,a5
   1033c:	0017871b          	addiw	a4,a5,1
   10340:	0007069b          	sext.w	a3,a4
   10344:	00016737          	lui	a4,0x16
   10348:	d8d72023          	sw	a3,-640(a4) # 15d80 <SP>
   1034c:	000166b7          	lui	a3,0x16
   10350:	02079793          	slli	a5,a5,0x20
   10354:	0207d793          	srli	a5,a5,0x20
   10358:	00179713          	slli	a4,a5,0x1
   1035c:	98068793          	addi	a5,a3,-1664 # 15980 <STACK>
   10360:	00f707b3          	add	a5,a4,a5
   10364:	fee45703          	lhu	a4,-18(s0)
   10368:	00e79023          	sh	a4,0(a5)
   1036c:	0cc0006f          	j	10438 <compile_bf+0x288>
   10370:	000167b7          	lui	a5,0x16
   10374:	d807a783          	lw	a5,-640(a5) # 15d80 <SP>
   10378:	00079663          	bnez	a5,10384 <compile_bf+0x1d4>
   1037c:	00100793          	li	a5,1
   10380:	1380006f          	j	104b8 <compile_bf+0x308>
   10384:	000167b7          	lui	a5,0x16
   10388:	d807a783          	lw	a5,-640(a5) # 15d80 <SP>
   1038c:	fff7879b          	addiw	a5,a5,-1
   10390:	0007871b          	sext.w	a4,a5
   10394:	000167b7          	lui	a5,0x16
   10398:	d8e7a023          	sw	a4,-640(a5) # 15d80 <SP>
   1039c:	000167b7          	lui	a5,0x16
   103a0:	d807a703          	lw	a4,-640(a5) # 15d80 <SP>
   103a4:	000167b7          	lui	a5,0x16
   103a8:	02071713          	slli	a4,a4,0x20
   103ac:	02075713          	srli	a4,a4,0x20
   103b0:	00171713          	slli	a4,a4,0x1
   103b4:	98078793          	addi	a5,a5,-1664 # 15980 <STACK>
   103b8:	00f707b3          	add	a5,a4,a5
   103bc:	0007d783          	lhu	a5,0(a5)
   103c0:	fef41123          	sh	a5,-30(s0)
   103c4:	fee45783          	lhu	a5,-18(s0)
   103c8:	0007871b          	sext.w	a4,a5
   103cc:	000127b7          	lui	a5,0x12
   103d0:	00271713          	slli	a4,a4,0x2
   103d4:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   103d8:	00f707b3          	add	a5,a4,a5
   103dc:	00800713          	li	a4,8
   103e0:	00e79023          	sh	a4,0(a5)
   103e4:	fee45783          	lhu	a5,-18(s0)
   103e8:	0007879b          	sext.w	a5,a5
   103ec:	00012737          	lui	a4,0x12
   103f0:	98070713          	addi	a4,a4,-1664 # 11980 <_edata>
   103f4:	00279793          	slli	a5,a5,0x2
   103f8:	00f707b3          	add	a5,a4,a5
   103fc:	fe245703          	lhu	a4,-30(s0)
   10400:	00e79123          	sh	a4,2(a5)
   10404:	fe245783          	lhu	a5,-30(s0)
   10408:	0007879b          	sext.w	a5,a5
   1040c:	00012737          	lui	a4,0x12
   10410:	98070713          	addi	a4,a4,-1664 # 11980 <_edata>
   10414:	00279793          	slli	a5,a5,0x2
   10418:	00f707b3          	add	a5,a4,a5
   1041c:	fee45703          	lhu	a4,-18(s0)
   10420:	00e79123          	sh	a4,2(a5)
   10424:	0140006f          	j	10438 <compile_bf+0x288>
   10428:	fee45783          	lhu	a5,-18(s0)
   1042c:	fff7879b          	addiw	a5,a5,-1
   10430:	fef41723          	sh	a5,-18(s0)
   10434:	00000013          	nop
   10438:	fe842783          	lw	a5,-24(s0)
   1043c:	0017879b          	addiw	a5,a5,1
   10440:	fef42423          	sw	a5,-24(s0)
   10444:	fee45783          	lhu	a5,-18(s0)
   10448:	0017879b          	addiw	a5,a5,1
   1044c:	fef41723          	sh	a5,-18(s0)
   10450:	fe842703          	lw	a4,-24(s0)
   10454:	fd442783          	lw	a5,-44(s0)
   10458:	0007071b          	sext.w	a4,a4
   1045c:	0007879b          	sext.w	a5,a5
   10460:	00f75a63          	ble	a5,a4,10474 <compile_bf+0x2c4>
   10464:	fee45783          	lhu	a5,-18(s0)
   10468:	0007871b          	sext.w	a4,a5
   1046c:	000017b7          	lui	a5,0x1
   10470:	d6f762e3          	bltu	a4,a5,101d4 <compile_bf+0x24>
   10474:	000167b7          	lui	a5,0x16
   10478:	d807a783          	lw	a5,-640(a5) # 15d80 <SP>
   1047c:	00079a63          	bnez	a5,10490 <compile_bf+0x2e0>
   10480:	fee45783          	lhu	a5,-18(s0)
   10484:	0007871b          	sext.w	a4,a5
   10488:	000017b7          	lui	a5,0x1
   1048c:	00f71663          	bne	a4,a5,10498 <compile_bf+0x2e8>
   10490:	00100793          	li	a5,1
   10494:	0240006f          	j	104b8 <compile_bf+0x308>
   10498:	fee45783          	lhu	a5,-18(s0)
   1049c:	0007871b          	sext.w	a4,a5
   104a0:	000127b7          	lui	a5,0x12
   104a4:	00271713          	slli	a4,a4,0x2
   104a8:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   104ac:	00f707b3          	add	a5,a4,a5
   104b0:	00079023          	sh	zero,0(a5)
   104b4:	00000793          	li	a5,0
   104b8:	00078513          	mv	a0,a5
   104bc:	02813403          	ld	s0,40(sp)
   104c0:	03010113          	addi	sp,sp,48
   104c4:	00008067          	ret

00000000000104c8 <execute_bf>:
   104c8:	fe010113          	addi	sp,sp,-32
   104cc:	00113c23          	sd	ra,24(sp)
   104d0:	00813823          	sd	s0,16(sp)
   104d4:	02010413          	addi	s0,sp,32
   104d8:	fe041723          	sh	zero,-18(s0)
   104dc:	000107b7          	lui	a5,0x10
   104e0:	fff7879b          	addiw	a5,a5,-1
   104e4:	fef42423          	sw	a5,-24(s0)
   104e8:	01c0006f          	j	10504 <execute_bf+0x3c>
   104ec:	000167b7          	lui	a5,0x16
   104f0:	fe846703          	lwu	a4,-24(s0)
   104f4:	00171713          	slli	a4,a4,0x1
   104f8:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   104fc:	00f707b3          	add	a5,a4,a5
   10500:	00079023          	sh	zero,0(a5)
   10504:	fe842783          	lw	a5,-24(s0)
   10508:	fff7879b          	addiw	a5,a5,-1
   1050c:	fef42423          	sw	a5,-24(s0)
   10510:	fe842783          	lw	a5,-24(s0)
   10514:	0007879b          	sext.w	a5,a5
   10518:	fc079ae3          	bnez	a5,104ec <execute_bf+0x24>
   1051c:	1cc0006f          	j	106e8 <execute_bf+0x220>
   10520:	fee45783          	lhu	a5,-18(s0)
   10524:	0007871b          	sext.w	a4,a5
   10528:	000127b7          	lui	a5,0x12
   1052c:	00271713          	slli	a4,a4,0x2
   10530:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   10534:	00f707b3          	add	a5,a4,a5
   10538:	0007d783          	lhu	a5,0(a5)
   1053c:	0007879b          	sext.w	a5,a5
   10540:	00078693          	mv	a3,a5
   10544:	00800713          	li	a4,8
   10548:	18d76063          	bltu	a4,a3,106c8 <execute_bf+0x200>
   1054c:	00279713          	slli	a4,a5,0x2
   10550:	000117b7          	lui	a5,0x11
   10554:	94c78793          	addi	a5,a5,-1716 # 1094c <_start+0x170>
   10558:	00f707b3          	add	a5,a4,a5
   1055c:	0007a783          	lw	a5,0(a5)
   10560:	00078067          	jr	a5
   10564:	fe842783          	lw	a5,-24(s0)
   10568:	0017879b          	addiw	a5,a5,1
   1056c:	fef42423          	sw	a5,-24(s0)
   10570:	16c0006f          	j	106dc <execute_bf+0x214>
   10574:	fe842783          	lw	a5,-24(s0)
   10578:	fff7879b          	addiw	a5,a5,-1
   1057c:	fef42423          	sw	a5,-24(s0)
   10580:	15c0006f          	j	106dc <execute_bf+0x214>
   10584:	000167b7          	lui	a5,0x16
   10588:	fe846703          	lwu	a4,-24(s0)
   1058c:	00171713          	slli	a4,a4,0x1
   10590:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   10594:	00f707b3          	add	a5,a4,a5
   10598:	0007d783          	lhu	a5,0(a5)
   1059c:	0017879b          	addiw	a5,a5,1
   105a0:	03079713          	slli	a4,a5,0x30
   105a4:	03075713          	srli	a4,a4,0x30
   105a8:	000167b7          	lui	a5,0x16
   105ac:	fe846683          	lwu	a3,-24(s0)
   105b0:	00169693          	slli	a3,a3,0x1
   105b4:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   105b8:	00f687b3          	add	a5,a3,a5
   105bc:	00e79023          	sh	a4,0(a5)
   105c0:	11c0006f          	j	106dc <execute_bf+0x214>
   105c4:	000167b7          	lui	a5,0x16
   105c8:	fe846703          	lwu	a4,-24(s0)
   105cc:	00171713          	slli	a4,a4,0x1
   105d0:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   105d4:	00f707b3          	add	a5,a4,a5
   105d8:	0007d783          	lhu	a5,0(a5)
   105dc:	fff7879b          	addiw	a5,a5,-1
   105e0:	03079713          	slli	a4,a5,0x30
   105e4:	03075713          	srli	a4,a4,0x30
   105e8:	000167b7          	lui	a5,0x16
   105ec:	fe846683          	lwu	a3,-24(s0)
   105f0:	00169693          	slli	a3,a3,0x1
   105f4:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   105f8:	00f687b3          	add	a5,a3,a5
   105fc:	00e79023          	sh	a4,0(a5)
   10600:	0dc0006f          	j	106dc <execute_bf+0x214>
   10604:	000167b7          	lui	a5,0x16
   10608:	fe846703          	lwu	a4,-24(s0)
   1060c:	00171713          	slli	a4,a4,0x1
   10610:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   10614:	00f707b3          	add	a5,a4,a5
   10618:	0007d783          	lhu	a5,0(a5)
   1061c:	0007879b          	sext.w	a5,a5
   10620:	00078513          	mv	a0,a5
   10624:	a8dff0ef          	jal	ra,100b0 <putchar>
   10628:	0b40006f          	j	106dc <execute_bf+0x214>
   1062c:	000167b7          	lui	a5,0x16
   10630:	fe846703          	lwu	a4,-24(s0)
   10634:	00171713          	slli	a4,a4,0x1
   10638:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   1063c:	00f707b3          	add	a5,a4,a5
   10640:	00079023          	sh	zero,0(a5)
   10644:	0980006f          	j	106dc <execute_bf+0x214>
   10648:	000167b7          	lui	a5,0x16
   1064c:	fe846703          	lwu	a4,-24(s0)
   10650:	00171713          	slli	a4,a4,0x1
   10654:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   10658:	00f707b3          	add	a5,a4,a5
   1065c:	0007d783          	lhu	a5,0(a5)
   10660:	06079863          	bnez	a5,106d0 <execute_bf+0x208>
   10664:	fee45783          	lhu	a5,-18(s0)
   10668:	0007879b          	sext.w	a5,a5
   1066c:	00012737          	lui	a4,0x12
   10670:	98070713          	addi	a4,a4,-1664 # 11980 <_edata>
   10674:	00279793          	slli	a5,a5,0x2
   10678:	00f707b3          	add	a5,a4,a5
   1067c:	0027d783          	lhu	a5,2(a5)
   10680:	fef41723          	sh	a5,-18(s0)
   10684:	04c0006f          	j	106d0 <execute_bf+0x208>
   10688:	000167b7          	lui	a5,0x16
   1068c:	fe846703          	lwu	a4,-24(s0)
   10690:	00171713          	slli	a4,a4,0x1
   10694:	d8878793          	addi	a5,a5,-632 # 15d88 <data>
   10698:	00f707b3          	add	a5,a4,a5
   1069c:	0007d783          	lhu	a5,0(a5)
   106a0:	02078c63          	beqz	a5,106d8 <execute_bf+0x210>
   106a4:	fee45783          	lhu	a5,-18(s0)
   106a8:	0007879b          	sext.w	a5,a5
   106ac:	00012737          	lui	a4,0x12
   106b0:	98070713          	addi	a4,a4,-1664 # 11980 <_edata>
   106b4:	00279793          	slli	a5,a5,0x2
   106b8:	00f707b3          	add	a5,a4,a5
   106bc:	0027d783          	lhu	a5,2(a5)
   106c0:	fef41723          	sh	a5,-18(s0)
   106c4:	0140006f          	j	106d8 <execute_bf+0x210>
   106c8:	00100793          	li	a5,1
   106cc:	0700006f          	j	1073c <execute_bf+0x274>
   106d0:	00000013          	nop
   106d4:	0080006f          	j	106dc <execute_bf+0x214>
   106d8:	00000013          	nop
   106dc:	fee45783          	lhu	a5,-18(s0)
   106e0:	0017879b          	addiw	a5,a5,1
   106e4:	fef41723          	sh	a5,-18(s0)
   106e8:	fee45783          	lhu	a5,-18(s0)
   106ec:	0007871b          	sext.w	a4,a5
   106f0:	000127b7          	lui	a5,0x12
   106f4:	00271713          	slli	a4,a4,0x2
   106f8:	98078793          	addi	a5,a5,-1664 # 11980 <_edata>
   106fc:	00f707b3          	add	a5,a4,a5
   10700:	0007d783          	lhu	a5,0(a5)
   10704:	00078c63          	beqz	a5,1071c <execute_bf+0x254>
   10708:	fe842783          	lw	a5,-24(s0)
   1070c:	0007871b          	sext.w	a4,a5
   10710:	000107b7          	lui	a5,0x10
   10714:	ffe78793          	addi	a5,a5,-2 # fffe <putchar-0xb2>
   10718:	e0e7f4e3          	bleu	a4,a5,10520 <execute_bf+0x58>
   1071c:	fe842783          	lw	a5,-24(s0)
   10720:	0007871b          	sext.w	a4,a5
   10724:	000107b7          	lui	a5,0x10
   10728:	fff78793          	addi	a5,a5,-1 # ffff <putchar-0xb1>
   1072c:	40f707b3          	sub	a5,a4,a5
   10730:	0017b793          	seqz	a5,a5
   10734:	0ff7f793          	andi	a5,a5,255
   10738:	0007879b          	sext.w	a5,a5
   1073c:	00078513          	mv	a0,a5
   10740:	01813083          	ld	ra,24(sp)
   10744:	01013403          	ld	s0,16(sp)
   10748:	02010113          	addi	sp,sp,32
   1074c:	00008067          	ret

0000000000010750 <main>:
   10750:	fe010113          	addi	sp,sp,-32
   10754:	00113c23          	sd	ra,24(sp)
   10758:	00813823          	sd	s0,16(sp)
   1075c:	02010413          	addi	s0,sp,32
   10760:	000127b7          	lui	a5,0x12
   10764:	9787b783          	ld	a5,-1672(a5) # 11978 <hello_world>
   10768:	fef43023          	sd	a5,-32(s0)
   1076c:	fe043503          	ld	a0,-32(s0)
   10770:	9e9ff0ef          	jal	ra,10158 <strlen>
   10774:	00050793          	mv	a5,a0
   10778:	0007879b          	sext.w	a5,a5
   1077c:	00078593          	mv	a1,a5
   10780:	fe043503          	ld	a0,-32(s0)
   10784:	a2dff0ef          	jal	ra,101b0 <compile_bf>
   10788:	00050793          	mv	a5,a0
   1078c:	fef42623          	sw	a5,-20(s0)
   10790:	fec42783          	lw	a5,-20(s0)
   10794:	0007879b          	sext.w	a5,a5
   10798:	00079863          	bnez	a5,107a8 <main+0x58>
   1079c:	d2dff0ef          	jal	ra,104c8 <execute_bf>
   107a0:	00050793          	mv	a5,a0
   107a4:	fef42623          	sw	a5,-20(s0)
   107a8:	fec42783          	lw	a5,-20(s0)
   107ac:	0007871b          	sext.w	a4,a5
   107b0:	00100793          	li	a5,1
   107b4:	00f71863          	bne	a4,a5,107c4 <main+0x74>
   107b8:	000117b7          	lui	a5,0x11
   107bc:	97078513          	addi	a0,a5,-1680 # 10970 <_start+0x194>
   107c0:	931ff0ef          	jal	ra,100f0 <puts>
   107c4:	fec42783          	lw	a5,-20(s0)
   107c8:	00078513          	mv	a0,a5
   107cc:	01813083          	ld	ra,24(sp)
   107d0:	01013403          	ld	s0,16(sp)
   107d4:	02010113          	addi	sp,sp,32
   107d8:	00008067          	ret

00000000000107dc <_start>:
   107dc:	00002197          	auipc	gp,0x2
   107e0:	99c18193          	addi	gp,gp,-1636 # 12178 <__global_pointer$>
   107e4:	00037137          	lui	sp,0x37
   107e8:	d8810113          	addi	sp,sp,-632 # 36d88 <_end>
   107ec:	ff010113          	addi	sp,sp,-16
   107f0:	f61ff0ef          	jal	ra,10750 <main>
   107f4:	26a02c23          	sw	a0,632(zero) # 278 <putchar-0xfe38>
   107f8:	00000013          	nop
   107fc:	00000013          	nop
   10800:	00000013          	nop
   10804:	00000013          	nop
   10808:	00000013          	nop
   1080c:	00000013          	nop
