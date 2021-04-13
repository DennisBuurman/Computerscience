/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include <iostream>
#include <functional>
#include <map>
#include "inst-decoder.h"

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

//Parsed through the last n bits of inst and gives those back
uint32_t parser (uint32_t & inst, int n){
	int modulo = 1;
	uint32_t temp = inst;

	for (int i = 0; i < n; i++){
		modulo *= 2;
	} //for
	inst>>=n;

	return temp % modulo;
} //parser

//Determines the amount of bits of a result, which will be needed for performLoad and performStore
//based on the funct3 value
uint8_t getSize(uint32_t size){
	switch(size){
		case 0: 
		case 4:
			size = 8;
		break;
		case 1: 
		case 5:
			size = 16;
		break;
		case 2: 
		case 6:
			size = 32;
		break;
		case 3:
			size = 64;
		break;
		default: //error
			throw IllegalInstruction("Size incorrect.");
		break;
	}//switch size
	return size;
}//getSize

//Split instructioncode for type R
void InstructionDecoder::splitR(uint32_t temp){
   decoded.rd = parser(temp, 5);
   decoded.funct3 = parser(temp, 3);
   decoded.rs1 = parser(temp, 5);
   decoded.rs2 = parser(temp, 5);
   decoded.funct7 = parser(temp, 7);
}//InstructionDecoder::splitR

//Split instructioncode for type I
void InstructionDecoder::splitI(uint32_t temp){
   decoded.rd = parser(temp, 5);
   decoded.funct3 = parser(temp, 3);
   decoded.rs1 = parser(temp, 5);
   decoded.imm = parser(temp, 12);
   decoded.length = 12;
   decoded.len_sl = getSize(decoded.funct3); //length needed for performLoad and performStore
}//InstructionDecoder::splitI

//Split instructioncode for type S
void InstructionDecoder::splitS(uint32_t temp){
	uint32_t imm1, imm2; //helpers to zip the two immediates into 1
	
	imm1 = parser(temp,5);
	decoded.funct3 = parser(temp, 3);
	decoded.rs1 = parser(temp, 5);
   decoded.rs2 = parser(temp, 5);
   imm2 = parser(temp, 7);
   decoded.length = 12;
   decoded.len_sl = getSize(decoded.funct3); //length needed for performLoad and performStore
   
   imm2 <<= 5;
   decoded.imm = imm1 + imm2;
}//InstructionDecoder::splitS

//Split instructioncode for type U
void InstructionDecoder::splitU(uint32_t temp){
	decoded.rd = parser(temp, 5);
	decoded.imm = parser(temp, 20);
	decoded.imm <<= 12;
	decoded.length = 32;
}//InstructionDecoder::splitU

//Split instructioncode for type J
void InstructionDecoder::splitJ(uint32_t temp){
	uint32_t imm1, imm2, imm3, imm4, immr; //helpers to zip the two immediates into 1
	decoded.rd = parser(temp, 5);
	decoded.length = 21;
	
	imm1 = parser(temp, 8);
	imm2 = parser(temp, 1);
	imm3 = parser(temp, 10);
	imm4 = parser(temp, 1);
	
	//zipping the parts of the imm to one
	immr = 0;
	immr |= (imm1 << 12);
	immr |= (imm2 << 11);
	immr |= (imm3 << 1);
	immr |= (imm4 << 20);
	
	decoded.imm = immr;
}//InstructionDecoder::splitJ

//Split instructioncode for type B
void InstructionDecoder::splitB(uint32_t temp){
	uint32_t imm1, imm2, imm3, imm4, immr; //helpers to zip the two immediates into 1
	
	imm1 = parser(temp, 1);
	imm2 = parser(temp, 4);
	decoded.funct3 = parser(temp, 3);
	decoded.rs1 = parser(temp, 5);
	decoded.rs2 = parser(temp, 5);
	imm3 = parser(temp, 6);
	imm4 = parser(temp, 1);
	decoded.length = 13;
	
	//zipping the parts of the imm to one
	immr = 0;
	immr |= (imm1 << 11);
	immr |= (imm2 << 1);
	immr |= (imm3 << 5);
	immr |= (imm4 << 12);
	
	decoded.imm = immr;	
}//InstructionDecoder::splitB


/* Decodes a single instruction. The decoded instruction should be
 * stored in the class member "decoded" of type DecodedInstruction.
 */
void InstructionDecoder::decodeInstruction(const uint32_t instruction) {
   uint32_t temp = instruction;
   uint32_t opcode = parser(temp, 7);  
   decoded.opcode = opcode;
   switch(opcode){
   	case 51: //0110011
   		decoded.type = R;
   		decoded.address = false;
   		decoded.load = false;
   		splitR(temp);
   		break;
   	case 3: //0000011
   		decoded.type = I;
   		decoded.address = true;
   		decoded.load = true;
   		splitI(temp);
   		break;
   	case 35: //0100011
   		decoded.type = S;
   		decoded.address = true;
   		decoded.load = false;
   		splitS(temp);
   		break;
   	case 19: //0010011
   		decoded.address = false;
   		decoded.load = false;
   		decoded.rd = parser(temp, 5);
   		decoded.funct3 = parser(temp, 3);
   		decoded.rs1 = parser(temp, 5);
   		if (decoded.funct3 == 1 || decoded.funct3 == 5){ //R-type
   			decoded.type = R;
   			decoded.imm = parser(temp, 6); //shamt
   			decoded.length = 6;
   			decoded.funct7 = parser(temp, 6);
   		} else { //I-type
   			decoded.type = I;
   			decoded.imm = parser(temp, 12);
  	 			decoded.length = 12;
   		} //if	
   		break;
   	case 23: //0010111
   		decoded.type = U;
   		decoded.address = false;
   		decoded.load = false;
   		splitU(temp);
   		break;
   	case 55: //0110111
   		decoded.type = U;
   		decoded.address = false;
   		decoded.load = false;
   		splitU(temp);
   		break;
   	case 111: //1101111
   		decoded.type = J;
   		decoded.address = false;
   		decoded.load = false;
   		splitJ(temp);
   		break;
   	case 103: //1100111
   		decoded.type = I;
   		decoded.address = false;
   		decoded.load = false;
   		splitI(temp);
   		break;
   	case 99: //1100011
   		decoded.type = B;
   		decoded.address = false;
   		decoded.load = false;
   		splitB(temp);
   		break;
   	//case 27 is special, because the instructions are from two different types
   	case 27: //0011011
   		decoded.rd = parser(temp, 5);
   		decoded.funct3 = parser(temp, 3);
   		decoded.address = false;
   		decoded.load = false;
   		if (decoded.funct3 == 0){ //ADDIW
   			decoded.type = I;
   			decoded.rs1 = parser(temp, 5);
   			decoded.imm = parser(temp, 12);
   			decoded.length = 12;
   		} else { //SLLIW, SRLIW, SRAIW
   			decoded.type = R;
   			decoded.rs1 = parser(temp, 5);
   			decoded.imm = parser(temp, 6);
   			decoded.length = 6;
   			decoded.funct7 = parser(temp, 6); 			
   		} //if
   		break;
   	case 59: //0111011
   		decoded.type = R;
   		decoded.address = false;
   		decoded.load = false;
   		splitR(temp);
   		break;
   	default: //error
   		throw IllegalInstruction("Instruction incorrect. Opcode wrong");
   		break;
   } //switch opcode
} //InstructionDecoder::decodeInstruction

DecodedInstruction InstructionDecoder::getDecodedInstruction(void) const{
  return decoded;
} //InstructionDecoder::getDecodedInstruction
