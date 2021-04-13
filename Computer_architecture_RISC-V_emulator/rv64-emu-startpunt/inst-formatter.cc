/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>
#include <sstream>
#include <bitset>

//Couts the used content of the decoder in debug mode

/* Output operator to print DecodedInstructions. */
std::ostream &
operator<<(std::ostream &os, const DecodedInstruction &decoded)
{
	std::cout << ": PC+4" << std::endl;
	std::cout << "----------------" << std::endl;
	switch(decoded.opcode){
   	case 51: //0110011
   		switch(decoded.funct3){
   			case 0: //ADD of SUB
   				switch(decoded.funct7){
   					case 0:  //ADD
   						std::cout << "ADD" << std::endl;
   						break;
   					case 32: //SUB
   						std::cout << "SUB" << std::endl;
   						break;	
   				}//ADD of SUB
   				break;
   			case 1:
   				std::cout << "SLL" << std::endl;
   				break;
   			case 4:
   				std::cout << "XOR" << std::endl;
   				break;
   			case 5:
   				std::cout << "SRL" << std::endl;
   				break;
   			case 6:
   				std::cout << "OR" << std::endl;
   				break;
   			case 7:
   				std::cout << "AND" << std::endl;
   				break;
   		}//switch funct3 van 0110011
   		break;
   	case 3: //0000011
   		switch(decoded.funct3){
  				case 0: //LB
  					std::cout << "LB" << std::endl;
  					break;
  				case 1: //LH
  					std::cout << "LH" << std::endl;
  					break;
  				case 2: //LW
  					std::cout << "LW" << std::endl;
  					break;
  				case 3: //LD
  					std::cout << "LD" << std::endl;
  					break;
  				case 4: //LBU 
  					std::cout << "LBU" << std::endl;
  					break;
  				case 5: //LHU
  					std::cout << "LHU" << std::endl;
  					break;
  				case 6: //LWU 
  					std::cout << "LWU" << std::endl;
  					break;
  			}//switch funct 3 0000011
   		break;
   	case 35: //0100011
   		switch(decoded.funct3){
  				case 0: //SB
  					std::cout << "SB" << std::endl;
  					break;
  				case 1: //SH
  					std::cout << "SH" << std::endl;
  					break;
  				case 2: //SW
  					std::cout << "SW" << std::endl;
  					break;
  				case 3: //SD
  					std::cout << "SD" << std::endl;
  					break;
  			}//switch funct 3 0100011
   		break;
   	case 19: //0010011
   		switch(decoded.funct3){
  				case 0: //ADDI
  					std::cout << "ADDI" << std::endl;
  					break;
  				case 1:
  					std::cout << "SLLI" << std::endl;
  					break;
  				case 2:
  					std::cout << "SLTI" << std::endl;
  					break;
  				case 3:
  					if (decoded.funct7 == 0){
  						std::cout << "SLTIU" << std::endl;
  					}
  					else{
  						std::cout << "SLTU" << std::endl;
  					}
  					break;
  				case 4:
  					std::cout << "XORI" << std::endl;
  					break;
  				case 5:
  					switch(decoded.funct7){
  						case 0:
  							std::cout << "SRLI" << std::endl;
  							break;
  						case 32:
  							std::cout << "SRAI" << std::endl;
  							break;
  					}//switch funct 7 0010011
  					break;
  				case 6:
  					std::cout << "ORI" << std::endl;
  					break;
  				case 7:
  					std::cout << "ANDI" << std::endl;
  					break;
  			}//switch funct 3 0010011
   		break;
   	case 23: //0010111
   		std::cout << "AUIPC" << std::endl;
   		break;
   	case 55: //0110111
   		std::cout << "LUI" << std::endl;
   		break;
   	case 111: //1101111
   		std::cout << "JAL" << std::endl;
   		break;
   	case 103: //1100111
   		std::cout << "JALR" << std::endl;
   		break;
   	case 99: //1100011
			switch(decoded.funct3){
				case 0: //BEQ
  					std::cout << "BEQ" << std::endl;
  					break;
  				case 1: //BNE
  					std::cout << "BNE" << std::endl;
  					break;
  				case 4: //BLT
  					std::cout << "BLT" << std::endl;
  					break;
  				case 5: //BGE
  					std::cout << "BGE" << std::endl;
  					break;
  				case 6: //BLTU
  					std::cout << "BLTU" << std::endl;
  					break;
  				case 7: //BGEU
  					std::cout << "BGEU" << std::endl;
  					break;
  			}//switch funct 3 1100011
   		break;
   	case 27: //0011011
   		switch(decoded.funct3){
   			case 0:
   				std::cout << "ADDIW" << std::endl;
   				break;
   			case 1:
   				std::cout << "SLLIW" << std::endl;
   				break;
   			case 5:
   				switch(decoded.funct7){
   					case 0:
   						std::cout << "SRLIW" << std::endl;
   						break;
   					case 32:
   						std::cout << "SRAIW" << std::endl;
   						break;
   				}//switch voor funct 7 0011011
   				break;
   		}//switch voor funct 3 0011011
   		break;
   	case 59: //0111011
   		switch(decoded.funct3){
   			case 0:
   				switch(decoded.funct7){
   					case 0:
   						std::cout << "ADDW" << std::endl;
   						break;
   					case 1:
   						std::cout << "MULW" << std::endl;
   						break;
   					case 32:
   						std::cout << "SUBW" << std::endl;
   				}//switch funct 7 van funct3=0 0111011
   				break;
   			case 1:
   				std::cout << "SLLW" << std::endl;
   				break;
   			case 5:
   				switch(decoded.funct7){
   					case 0:
   						std::cout << "SRLW" << std::endl;
   						break;
   					case 1:
   						std::cout << "DIVUW" << std::endl;
   						break;
   					case 32:
   						std::cout << "SRAW" << std::endl;
   						break;
   				}//switch funct7 voor funct3=5 0111011
   				break;
   			case 4:
   				std::cout << "DIVW" << std::endl;
   				break;
   			case 6:
   				std::cout << "REMW" << std::endl;
   				break;
   			case 7:
   				std::cout << "REMUW" << std::endl;
   				break;
   		}//switch voor funct 3 0111011 
   		break;
   }//switch voor opcodes
	std::cout << "Opcode: " << std::bitset<7> (decoded.opcode) << std::endl;
	if ((int)decoded.type != 5 && (int)decoded.type != 6){
		std::cout << "Funct3: " << decoded.funct3 << std::endl;
	}//if
	if ((int)decoded.type == 1){
		std::cout << "Funct7: " << decoded.funct7 << std::endl;
	}//if
	if ((int)decoded.type != 1){
		std::cout << "Imm:    " << decoded.imm << std::endl;
		std::cout << "Length: " << decoded.length << std::endl;
	}//if
	if ((int)decoded.type != 5 && (int)decoded.type != 6){
		std::cout << "rs1:    " << decoded.rs1 << std::endl;
	}//if
	if ((int)decoded.type != 5 && (int)decoded.type != 6 && (int)decoded.type != 2){
		std::cout << "rs2:    " << decoded.rs2 << std::endl;
	}//if
	if ((int)decoded.type != 3 && (int)decoded.type != 4){
		std::cout << "Rd:     " << decoded.rd << std::endl;
	}//if
	std::cout << "----------------" << std::endl;
	
   return os;
}
