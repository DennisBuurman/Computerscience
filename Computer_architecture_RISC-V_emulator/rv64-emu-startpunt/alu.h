/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include <map>
#include "inst-decoder.h"

/* The ALU component performs the specified operation on operands A and B,
 * placing the result in result. The operation is specified through
 * opcode and/or function code.
 */
class ALU {
  public:
  	 //setters for different parts of the instruction which are needed for the ALU
    void setA(RegValue A) { this->A = A; }
    void setB(RegValue B) { this->B = B; }
	 void setOpcode(uint32_t opcode_in) { this->opcode = opcode_in; }
	 void setFunct3(uint32_t funct3_in) { this->funct3 = funct3_in; }
	 void setFunct7(uint32_t funct7_in) { this->funct7 = funct7_in; }
	 void setType(instructionType type_in) { this->type = type_in; }
	 void setImm(uint32_t imm_in) { this->imm = imm_in; }
	 void setLength(int length_in) { this->length = length_in; }
	 void setRd(uint8_t rd_in) { this->rd = rd_in; }

	 //getter for computed result of ALU
    RegValue getResult() const { return result; }

    void execute();

  private:
    RegValue A;
    RegValue B;
    RegValue result;
    
    instructionType type;
    
    //elements of one instruction needed for ALU
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t imm;
    uint8_t rd; 
    
    //length of the immediate, needed for signExtend
    int length;
}; //ALU

#endif /* __ALU_H__ */
