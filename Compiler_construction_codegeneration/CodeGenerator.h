/*
 * CodeGenerator.h - Declaration of the CodeGenerator class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#ifndef _CODEGENERATOR_H_
#define _CODEGENERATOR_H_

#include <vector>
#include <map>
#include <utility>
#include <string>
#include <queue>
#include <stdlib.h>
#include <time.h>

#include <cstdio>
#include "SymbolTable.h"
#include "IntermediateCode.h"
#include "StackFrameManager.h"

//number of registers integer/float
int const numRegs = 31;

//integer register constants
int const argRegsBegin = 3;
int const freeRegsBegin = 8;
int const freeRegsEnd = 25;

//float register constants
int const fRegsBegin = 1;
int const syscallArg = 12;

//register spill amount
int const numWrites = 5;

// This class handles the assembly code generation. Extend it to your own
// needs.
class CodeGenerator {
  public:
    // Constructor
    CodeGenerator();

    // Destructor
    ~CodeGenerator();

    // Generates a header
    void GenerateHeader(FILE * out);
    
    // Generates the declarations for the global variables
    void GenerateGlobalDecls(FILE * out, SymbolTable * symtab);
    
    // Takes an IntermediateCode object and emits MIPS assembly instructions
    void GenerateCode(FILE * out, SymbolTable * symtab, IntermediateCode * inputCode);

    // Generates a trailer
    void GenerateTrailer(FILE * out);
    
  private:
    // vector of marked variables in current scope
    std::vector<Symbol*> mark;
    // vector of marked globals
    std::vector<Symbol*> markGlobal;
    // vector of param operands 
    std::vector<IOperand*> paramVec;
    // vector of labels to determine branch scope
    std::vector<Symbol*> labelWatch;
    // vector of variables within branch
    std::vector<Symbol*> varWatch;
    
    // The current stack frame:
    StackFrameManager currentFrame;

    // ... your own private members and methods ...
    // makes the icode reachable within the object
    IntermediateCode* icode;
    // counting the instructions
    int inst_count;
    
    // keeps track of register allocation 
    Symbol* int_regs[numRegs];
    bool int_allocated[numRegs];
    
    // keeps track of float register allocation
    Symbol* float_regs[numRegs];
    bool float_allocated[numRegs];
    
    // returns the instruction corresponding to the IOperator op
    std::string GetInstructionName(IOperator op);
    // returns the inverse of the branching instruction in IOperator op
    std::string ReverseBranch(IOperator op);
    // returns the flag (bc1t/bc1f) of a float branch instruction
    std::string branch_instruction(IOperator op);
    
    // returns if a symbol is already marked; on false: mark symbol sym
    bool marked(Symbol* sym);
    // returns if the symbol is alive after the current instruction
    bool is_live(Symbol* sym);
    
    // ---Register operations---
    // returns register number where sym is found; -1 on not found
    int find_reg_f(Symbol* sym);
    int find_reg_i(Symbol* sym);
    
    // returns register number where sym is allocated; -1 on registers full
    int set_reg_f(Symbol* sym);
    int set_reg_i(Symbol* sym);
    
    // returns true if IOperand op is an immediate
    bool is_imm(IOperand* op);
    
    // sets an immediate value to a register
    int imm_reg_i(FILE* out, int val);
    int imm_reg_f(FILE* out, float val);
    
    // randomly spills numWrites allocated usable registers to the stack
    // _f_: float registers; _i_: integer registers
    void write_f_to_memory(FILE* out);
    void write_i_to_memory(FILE* out);
    
    // deallocates register number reg
    // _f: float register reg; _i: integer register reg
    int deallocate_f(int reg);
    int deallocate_i(int reg);
    
    // reclaims unused registers or spills some registers to create space
    // _f: float registers; _i: integer registers
    int collect_f(FILE* out);
    int collect_i(FILE* out);
    
    // allocate a register for Symbol sym and return it as a string
    // _f: allocate float register; _i: allocate integer register
    std::string allocate_f(FILE* out, Symbol* sym);
    std::string allocate_i(FILE* out, Symbol* sym);
    
    // deallocate variables used within a branch if they are not live after branch
    void deallocate_branch_regs(FILE* out, Symbol* sym);
    
    // deallocates local variables and parameters of subprogram funcSym if they are not alive
    void deallocate_locals(FILE* out, SymbolTable* symtab, Symbol* funcSym);
    
    // stores Symbol sym on the stack 
    void store(FILE* out, Symbol* sym);
    
    // ---Calls---
    // Add a parameter to the paramVec 
    void write_parameter(FILE* out, SymbolTable* symtab, IStatement* stmt);
    // places parameters from paramVec on stack and performs a procedure call
    void write_procedurecall(FILE* out, SymbolTable* symtab, IStatement* stmt);
    // places parameters from paramVec on stack, performs function call, and saves return value in $2
    void write_functioncall(FILE* out, SymbolTable* symtab, IStatement* stmt);
    
    // ---Unary---
    // handles the unary IOP_NOT and IOP_UNARY_MINUS_I instructions
    void write_unary(FILE* out, SymbolTable* symtab, IStatement* stmt);
    // handles the unary IOP_UNARY_MINUS_R instruction
    void write_unary_f(FILE* out, SymbolTable* symtab, IStatement* stmt);
    // handles a coercion instruction
    void write_coercion(FILE* out, SymbolTable* symtab, IStatement* stmt);
    
    // ---Jump, label---
    // handles a jump instruction
    void write_jump(FILE* out, SymbolTable* symtab, IStatement* stmt);
    // handles a label instruction
    void write_label(FILE* out, SymbolTable* symtab, IStatement* stmt);
    
    // ---Rest---
    // handles arithmetic instructions
    // _i: IOP_ADD_I, IOP_SUB_I, IOP_MUL_I, IOP_DIV_I, IOP_MOD, IOP_AND, IOP_OR
    // _f: IOP_ADD_R, IOP_SUB_R, IOP_MUL_R, IOP_DIV_R:
    void write_arithmetic_i(FILE* out, SymbolTable* symtab, IStatement* stmt);
    void write_arithmetic_f(FILE* out, SymbolTable* symtab, IStatement* stmt);
    
    // handles assignment operations
    // _i: IOP_ASSIGN_I; _f: IOP_ASSIGN_R
    void write_assign_i(FILE* out, SymbolTable* symtab, IStatement* stmt);
    void write_assign_f(FILE* out, SymbolTable* symtab, IStatement* stmt);
    
    // handles branch instructions
    // _i: IOP_BEQ_I, IOP_BLT_I, IOP_BGT_I, IOP_BLE_I, IOP_BGE_I, IOP_BNE_I
    // _f: IOP_BEQ_R, IOP_BLT_R, IOP_BGT_R, IOP_BLE_R, IOP_BGE_R, IOP_BNE_R
    void write_branch_i(FILE* out, SymbolTable* symtab, IStatement* stmt);
    void write_branch_f(FILE* out, SymbolTable* symtab, IStatement* stmt);
};

#endif
