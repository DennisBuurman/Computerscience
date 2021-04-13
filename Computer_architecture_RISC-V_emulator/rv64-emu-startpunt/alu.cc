/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
*/
 
#include <math.h>
#include <iostream>
#include "alu.h"

//Convert value to 2s complement
uint64_t to2sComplement(uint64_t value){
	uint64_t imm2 = value;
	imm2 = ~imm2;
	imm2++;
	return imm2;
} //to2sComplement

//Sign extend the value to uint64_t
uint64_t signExtend(uint32_t input, int length){
	uint64_t output;
	if (input >> (length - 1) == 1){
		output = (0xffffffffffffffff << length) | input;
	} else {
		output = input;
	}//if
	return output;
} //signExtend

//Perform addition between number1 and number2
uint64_t ADD(uint64_t number1, uint64_t number2){
	uint64_t additionresult;
	additionresult = to2sComplement(number1) + to2sComplement(number2);
	return to2sComplement(additionresult);
} //ADD

//Based on the opcode, funct3 and funct7 determines the instruction which will be computed
//by the ALU. The computed address or result will then be put into variable result.
void ALU::execute(void){	
  	switch(opcode){
  		case 51: //0110011
  			switch(funct3){
   			case 0:
   				switch(funct7){
   					case 0: //ADD
   						result = ADD(A, B);
   						break;
   					case 1: //MUL
   						result = (uint64_t)A * (uint64_t)B;
   						break;
   					case 32: //SUB
   						result = ADD(A, to2sComplement(B));
   						break;
   					default: //error
   						throw IllegalInstruction("Funct7: Instruction incorrect. Opcode: 0110011");
   						break;	
   				} //switch funct7
   				break;
   			case 1: //SLL
   				result = A << B;
   				break;
   			case 3: //SLTU
   				result = (uint32_t)A < (uint32_t)B;
   				break;
   			case 4: //XOR
   				result = A ^ B;
   				break;
   			case 5:
   				switch(funct7){
   					case 0: //SRL
   						result = A >> B;
   						break;
   					default: //error
   						throw IllegalInstruction("Funct7: Instruction incorrect. Opcode: 0110011");
   						break;
   				} //switch funct7
   				break;
   			case 6: //OR
   				result = A | B;
   				break;
   			case 7: //AND
   				result = A & B;
   				break;
   			default: //error
   				throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 0110011");
   				break;
   		} //switch funct3
  			break;
  		case 3: //0000011
  			switch(funct3){
  				case 0: //LB
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 1: //LH
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 2: //LW
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 3: //LD
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 4: //LBU
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 5: //LHU
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 6: //LWU
  					result = ADD(A, signExtend(imm, length));
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 0000011");
  					break;
  			} //switch funct3
  			break;
  		case 19: //0010011
  			switch(funct3){
  				case 0: //ADDI
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 1: //SLLI
  					result = A << signExtend(imm, length);
  					break;
  				case 3: //SLTIU
  					result = A < imm;
  					break;
  				case 5:
  					switch(funct7){
  						case 0: //SRLI
  							result = A >> signExtend(imm, length);
  							break;
  						default: //error
  							throw IllegalInstruction("Funct7: Instruction incorrect. Opcode: 0010011");
  							break;
  					} //switch funct7
  					break;
  				case 4: //XORI
  					result = A ^ signExtend(imm, length);
  					break;
  				case 7: //ANDI
  					result = A & signExtend(imm, length);
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 0010011");
  					break;
  			} //switch funct3
  			break;
  		case 35: //0100011
  			switch(funct3){
  				case 0: //SB
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 1: //SH
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 2: //SW
  					result = ADD(A, signExtend(imm, length));
  					break;
  				case 3: //SD
  					result = ADD(A, signExtend(imm, length));
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode 0100011");
  					break;
  			} //switch funct3
  			break;
  		case 23: //0010111 - AUIPC
  			result = ADD(A, signExtend(imm, length));
  			break;
  		case 55: //0110111 - LUI
  			result = signExtend(imm, length);
  			break;
  		case 111: //1101111 - JAL
  			result = ADD(A, signExtend(imm, length));
  			break;
  		case 103: //1100111 - JALR
  			switch(funct3){
  				case 0: //JALR
  					result = ADD(A, signExtend(imm, length)) & -2;
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode 1100111");
  					break;
  			} //switch funct3
  			break;
  		case 99: //1100011
  			switch(funct3){
				case 0: //BEQ
  					if (A == B){
  						result = signExtend(imm, length);
  					} else {	
  						result = 0;
  					} //if
  					break;
  				case 1: //BNE
  					if (A != B){
  						result = signExtend(imm, length);
  					} else {
  						result = 0;
  					} //if
  					break;
  				case 4: //BLT
  					if ((int64_t)(A) < (int64_t)(B)) {
  						result = signExtend(imm, length);
  					} else {
  						result = 0;
  					} //if
  					break;
  				case 5: //BGE
  					if ((int64_t)(A) >= (int64_t)(B)) {
  						result = signExtend(imm, length);
  					} else {
  						result = 0;
  					} //if
  					break;
  				case 6: //BLTU
  					if (A < B){
  						result = signExtend(imm, length);
  					} else {
  						result = 0;
  					} //if
  					break;
  				case 7: //BGEU
  					if (A >= B){
  						result = signExtend(imm, length);
  					} else {
  						result = 0;
  					} //if
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 1100011");
  					break;
  			} //switch funct3
  			break;
  		case 27: //0011011
  			switch(funct3){
  				case 0: //ADDIW
  					result = ADD(A, signExtend(imm, length));
  					break;
				case 1: //SLLIW
					result = A << signExtend(imm, length);
					break;
  				case 5: 
  					switch(funct7){
  						case 0: //SRLIW
  							result = (A >> signExtend(imm, length));
  							break;
  						case 16: //SRAIW
  							result = A >> imm;
  							break;
  						default: //error
  							throw IllegalInstruction("Funct7: Instruction incorrect. Opcode: 0011011");
  							break;
  					} //switch funct7
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 0011011");
  					break;
  			} //switch funct3
  			break;
  		case 59: //0111011
  			switch(funct3){
  				case 0: 
  					switch(funct7){
  						case 0: //ADDW
  							result = ADD(A, B);
  							break;
  						case 32: //SUBW
  							result = ADD(A, to2sComplement(B));
  							break;
  						default: //error
  							throw IllegalInstruction("Funct7: Instruction incorrect. Opcode: 0111011");
  							break;
  					} //switch funct7
  					break;
  				case 6: //REMW
  					result = A % B;
  					break;
  				default: //error
  					throw IllegalInstruction("Funct3: Instruction incorrect. Opcode: 0111011");
  					break;
  			} //switch funct3
  			break;
  		default: //error
  			throw IllegalInstruction("Opcode: Instruction incorrect.");
  			break;
  	} //switch opcode
} //ALU::execute
