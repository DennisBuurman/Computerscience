/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include <stdexcept>
#include <cstdint>
#include "reg-file.h"

//Enum for determining the type of an instruction, numbers are assigned
enum instructionType{ R = 1, I = 2, S = 3, B = 4, U = 5, J = 6 };

/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error {
  public:
    explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
    { }

    explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
    { }
}; //IllegalInstruction

/* Structure to keep together all data for a single decoded instruction.
 */
struct DecodedInstruction {
  instructionType type;
  
  //elements of instruction needed for the ALU to determine what to do
  uint32_t opcode;
  uint32_t rd;
  uint32_t funct3;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t funct7;
  uint32_t imm;
  
  //length of result of the ALU needed for performStore and performLoad
  uint8_t len_sl;
  //length of the immediate needed for the function signExtend of ALU
  int length;
  //indicates if the function memory (of processor) has to do any loading/storing
  bool address;
  //if the boolean address is true, then this boolean determines if memory (of processor) has to load (T) or store (F)
  bool load;
}; //DecodedInstruction

std::ostream &operator<<(std::ostream &os, const DecodedInstruction &decoded);

/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder {
  public:
    void decodeInstruction(const uint32_t instruction);

    DecodedInstruction getDecodedInstruction(void) const;
	 
	 //Splitters for different types of instructions
	 void splitR (uint32_t temp);
	 void splitI (uint32_t temp);
	 void splitS (uint32_t temp);
	 void splitU (uint32_t temp);
	 void splitJ (uint32_t temp);
	 void splitB (uint32_t temp);

  private:
    DecodedInstruction decoded;
}; //InstructionDecoder

#endif /* __INST_DECODER_H__ */
