/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    processor.h - Processor class tying all components together.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "processor.h"
#include "inst-decoder.h"
#include "serial.h"

#include <iostream>
#include <iomanip>
#include <bitset>

class InstructionFetchFailure : public std::exception {
  public:
    explicit InstructionFetchFailure(const MemAddress addr)
    {
      std::stringstream ss;
      ss << "Instruction fetch failed at address " << std::hex << addr;
      message = ss.str();
    }

    virtual const char* what() const noexcept override
    {
      return message.c_str();
    }

  private:
    std::string message;
}; //InstructionFetchFailure

class TestEndMarkerEncountered : public std::exception {
  public:
    explicit TestEndMarkerEncountered(const MemAddress addr)
    {
      std::stringstream ss;
      ss << "Test end marker encountered at address " << std::hex << addr;
      message = ss.str();
    }

    virtual const char* what() const noexcept override
    {
      return message.c_str();
    }

  private:
    std::string message;
}; //TestEndMarkerEncountered


Processor::Processor(ELFFile &program, bool debugMode)
  : debugMode(debugMode), nCycles(0), nInstructions(0),
    PC(program.getEntrypoint()), bus(program.createMemories()),
  control(new SysControl(0x270))
{
  bus.addClient(std::shared_ptr<MemoryInterface>(new Serial(0x200)));
  bus.addClient(control);
} //Processor::Processor

/* This method is used to initialize registers using values
 * passed as command-line argument.
 */
void Processor::initRegister(RegNumber regnum, RegValue value) {
  regfile.writeRegister(regnum, value);
}//Processor::initRegister

//Used to get the value of a register with number regnum
RegValue Processor::getRegister(RegNumber regnum) const {
  return regfile.readRegister(regnum);
} //Processor::getRegister


/* Processor main loop. Each iteration should execute an instruction.
 * One step in executing and instruction takes 1 clock cycle.
 *
 * The return value indicates whether an error (exception) occurred during
 * execution (false) or whether the program was executed without problems
 * (true).
 *
 * In "testMode" instruction fetch failures are not fatal. This is because
 * a clean shutdown of the program requires the store instruction to be
 * implemented, so that the system controller can be informed. In unit tests,
 * we want to test as little instructions as possible and thus allow test
 * programs without store instruction to run without error.
 */
bool Processor::run(bool testMode) {
  while (! control->shouldHalt()){
      try
        {
          ++nCycles;
          instructionFetch();
			 
          ++nCycles;
          bool jumped = instructionDecode();
          if (jumped)
            continue;
			 
          ++nCycles;
          execute();

          ++nCycles;
          memory();

          ++nCycles;
          writeBack();
          ++nInstructions;
        }
      catch (TestEndMarkerEncountered &e)
        {
          if (testMode)
            return true;
          /* else */
          std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
                    << std::hex << PC << std::dec << std::endl;
          std::cerr << "Reason: " << e.what() << std::endl;
          return false;
        }
      catch (InstructionFetchFailure &e)
        {
          if (testMode)
            return true;
          /* else */
          std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
                    << std::hex << PC << std::dec << std::endl;
          std::cerr << "Reason: " << e.what() << std::endl;
          return false;
        }
      catch (std::exception &e)
        {
          /* Catch exceptions such as IllegalInstruction and InvalidAccess */
          std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
                    << std::hex << PC << std::dec << std::endl;
          std::cerr << "Reason1: " << e.what() << std::endl;
          return false;
        }
  } //while
  return true;
}//Processor::run

//Used to fetch the next instruction from the bus
//Also prematurely ups the PC by 4 bytes
void Processor::instructionFetch(void) {
  try
    {
      instruction = bus.readWord(PC);
      if (instruction == TestEndMarker)
        throw TestEndMarkerEncountered(PC);
	  	PC+=4; //upping the PC to its next instruction, unless the current one is a jump or branch
    }
  catch (TestEndMarkerEncountered &e)
    {
      throw;
    }
  catch (std::exception &e)
    {
      throw InstructionFetchFailure(PC);
    }
} //Processor::instructionFetch

/* Returns whether jump has already occurred during this step and further
 * steps can be skipped.
 */
bool Processor::instructionDecode(void) {
  decoder.decodeInstruction(instruction);

   if (debugMode){
      /* Dump program counter & decoded instruction in debug mode */
      auto storeFlags(std::cerr.flags());

      std::cerr << std::hex << std::showbase << PC << "\t";
      std::cerr.setf(storeFlags);
      std::cerr << decoder.getDecodedInstruction() << std::endl;
   }//if
   
   DecodedInstruction temp = decoder.getDecodedInstruction();
	
	//switch (type) determines what variables need to be passed through to the ALU
   switch (temp.type){
   	case R:
   		alu.setB(getRegister(temp.rs2));
   		alu.setRd(temp.rd);
   		alu.setFunct7(temp.funct7);
   		alu.setA(getRegister(temp.rs1));
   		alu.setFunct3(temp.funct3);
   		alu.setImm(temp.imm);
   		break;
   	case I:
   		alu.setRd(temp.rd);
   		alu.setImm(temp.imm);
   		alu.setA(getRegister(temp.rs1));
   		alu.setFunct3(temp.funct3);
   		break;
   	case S:
   		alu.setB(getRegister(temp.rs2));	
   	   alu.setImm(temp.imm);
   		alu.setA(getRegister(temp.rs1));
   		alu.setFunct3(temp.funct3);
   		alu.setRd(temp.rd);
   		break;
   	case B:
   		alu.setImm(temp.imm);
   		alu.setA(getRegister(temp.rs1));
   		alu.setB(getRegister(temp.rs2));
   		alu.setFunct3(temp.funct3);
   		break;
   	case U:
   		alu.setRd(temp.rd);
   		alu.setImm(temp.imm);
   		switch (temp.opcode){
   			case 23: //0010111 - AUIPC
   				alu.setA(PC-4); //PC is set prematurely, so PC-4 represents current PC
   				break;
   			case 55: //0110111 - LUI
   				break;
   			default: //error
   				throw IllegalInstruction("Opcode/Type incorrect");
   				break;
   		}//switch opcode
   		break;
   	case J:
   		alu.setRd(temp.rd);
   		alu.setA(PC-4); //PC is set prematurely, so PC-4 represents current PC
   		alu.setImm(temp.imm);
   		break;
   	default: //error
   		throw IllegalInstruction("Type incorrect");
   		break;
   }//switch type

   alu.setOpcode(temp.opcode);
   alu.setType(temp.type);
   alu.setLength(temp.length);
   
   return false;
} //Processor::instructionDecode

//Computing the address/result according to the instruction
void Processor::execute(void) {
  alu.execute();
} //Processor::execute

//Does the loading if needed, returns value of MemAddress
uint64_t Processor::performLoad(uint8_t size, bool signExtend, MemAddress addr) {
	switch(size){
		case 8:
			return bus.readByte(addr);
			break;
		case 16:
			return bus.readHalfWord(addr);
			break;
		case 32:
			return bus.readWord(addr);
			break;
		case 64:
			return bus.readDoubleWord(addr);
			break;
		default: //error
			throw IllegalInstruction("Load: Size is incorrect");
			break;
	} //switch size	
   return 0;
} //Processor::performLoad

//Does the storing if needed, puts value into the memory on address addr
void Processor::performStore(uint8_t size, RegValue value, MemAddress addr) {
	switch(size){
		case 8:
			return bus.writeByte(addr, value);
			break;
		case 16:
			return bus.writeHalfWord(addr, value);
			break;
		case 32:
			return bus.writeWord(addr, value);
			break;
		case 64:
			return bus.writeDoubleWord(addr, value);
			break;
		default: //error
			throw IllegalInstruction("Store: Size is incorrect");
			break;
	} //switch size
} //Processor::performStore

//Performs the access to the memory and determines with the booleans address and load what function to call
void Processor::memory(void) {
  if (decoder.getDecodedInstruction().address){
  		uint32_t address = alu.getResult();	
  		if (decoder.getDecodedInstruction().load){ //LOAD
  			regfile.writeRegister(decoder.getDecodedInstruction().rd, performLoad(decoder.getDecodedInstruction().len_sl, true, address));             
  		} else { //STORE
  			performStore(decoder.getDecodedInstruction().len_sl, regfile.readRegister(decoder.getDecodedInstruction().rs2), address);
  		} //if
  } //if   
} //Processor::memory

//Performs the writeback of the computed result of the ALU to the given register with registernumber rd
void Processor::writeBack(void) {
   if (!decoder.getDecodedInstruction().address){
		DecodedInstruction temp = decoder.getDecodedInstruction();
		result = alu.getResult();
		if (temp.opcode == 111){ //JAL
			regfile.writeRegister(temp.rd, PC);
			PC = result;
		} 
		else if (temp.opcode == 103){ //JALR
			regfile.writeRegister(temp.rd, PC);
			PC = result;
		}
		else if (temp.opcode == 99){ //Branch
		 	if (result != 0){
		 		PC = PC + result - 4;
		 	} //if
		}
		else {
			regfile.writeRegister(temp.rd, result);
		} //if
	} //if
} //Processor::writeBack

void Processor::dumpRegisters(void) const {
  const int NumColumns(2);
  const int valueFieldWidth(18);
  auto storeFlags(std::cerr.flags());

  for (int i = 0; i < NumRegs / NumColumns; ++i) {
      std::cerr << "R" << std::setw(2) << std::setfill('0') << i << " "
                << std::setw(valueFieldWidth)
                << std::hex << std::showbase
                << regfile.readRegister(i)
                << "\t";
      std::cerr.setf(storeFlags);
      std::cerr << "R" << std::setw(2) << (i + NumRegs/NumColumns) << " "
                << std::setw(valueFieldWidth)
                << std::hex << std::showbase
                << regfile.readRegister(i + NumRegs/NumColumns)
                << std::endl;
      std::cerr.setf(storeFlags);
  } //for
} //Processor::dumpRegisters

void Processor::dumpStatistics(void) const {
  std::cerr << nCycles << " clock cycles, "
            << nInstructions << " instructions executed." << std::endl
            << "CPI: " << ((float)nCycles / nInstructions) << std::endl;
} //Processor::dumpStatistics
