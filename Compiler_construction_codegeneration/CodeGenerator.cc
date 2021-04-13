/*
 * CodeGenerator.cc - Implementation of the CodeGenerator class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

// load store: marking
// liveness: empty assign removal

#include "CodeGenerator.h"

bool IsGlobal(Symbol * sym);  // Is the specified symbol a global variable?

// Constructor
CodeGenerator::CodeGenerator() {
}


// Destructor
CodeGenerator::~CodeGenerator() {
}

// some variable names cause errors within the spim emulator
inline std::string checkname(std::string str) {
  if (str == "b") {
    return "_b";
  } else if (str == "j") {
    return "_j"; 
  } else {
    return str;
  }
} //checkname

std::string CodeGenerator::GetInstructionName(IOperator op) {
  switch (op) {
    case IOP_GOTO:
      return "j";
      break;
    case IOP_ASSIGN_I:
      return "move";
      break;
    case IOP_ASSIGN_R:
      return "mov.s";
      break;
    case IOP_BEQ_I:
      return "beq";
      break;
    case IOP_BLT_I:
      return "blt";
      break;
    case IOP_BGT_I:
      return "bgt";
      break;
    case IOP_BLE_I:
      return "ble";
      break;
    case IOP_BGE_I:
      return "bge";
      break;
    case IOP_BNE_I:
      return "bne";
      break;
    case IOP_BEQ_R:
      return "c.eq.s"; // bc1t
      break;
    case IOP_BLT_R:
      return "c.lt.s"; // bc1t
      break;
    case IOP_BGT_R:
      return "c.le.s"; // bc1f
      break;
    case IOP_BLE_R:
      return "c.le.s"; // bc1t
      break;
    case IOP_BGE_R:
      return "c.lt.s"; // bc1f
      break;
    case IOP_BNE_R:
      return "c.eq.s"; // bc1f
      break;    
    case IOP_ADD_I:
      return "add"; // addi
    case IOP_ADD_R:
      return "add.s";
      break;
    case IOP_SUB_I:
      return "sub";
      break;
    case IOP_SUB_R:
      return "sub.s";
      break;
    case IOP_MUL_I:
      return "mul";
      break;
    case IOP_MUL_R:
      return "mul.s";
      break;
    case IOP_DIV_I:
      return "div";
      break;
    case IOP_DIV_R:
      return "div.s";
      break;
    case IOP_MOD:
      return "rem";
      break;
    case IOP_AND:
      return "and"; // andi
      break;
    case IOP_OR:
      return "or"; // ori
      break;   
    case IOP_NOT:
      return "not";
      break;
    case IOP_UNARY_MINUS_I:
      return "neg";
      break;
    case IOP_UNARY_MINUS_R:
      return "neg.s";
      break;
    case IOP_INT_TO_REAL:
      return "mtc1";
      break;
    default:
      return "UNKNOWN";
      break;
  }
  return "";
} //GetInstructionName

std::string CodeGenerator::ReverseBranch(IOperator op) {
  switch (op) {
    case IOP_BEQ_I:
      return "beq";
      break;
    case IOP_BLT_I:
      return "bgt";
      break;
    case IOP_BGT_I:
      return "blt";
      break;
    case IOP_BLE_I:
      return "bge";
      break;
    case IOP_BGE_I:
      return "ble";
      break;
    case IOP_BNE_I:
      return "bne";
      break;
    default: 
      return "ERROR UNKNOWN BRANCH";
      break;
  }
} //ReverseBranch

std::string CodeGenerator::branch_instruction(IOperator op) {
  switch(op) {
    case IOP_BEQ_R:
      return "bc1t"; // bc1t
      break;
    case IOP_BLT_R:
      return "bc1t"; // bc1t
      break;
    case IOP_BGT_R:
      return "bc1f"; // bc1f
      break;
    case IOP_BLE_R:
      return "bc1t"; // bc1t
      break;
    case IOP_BGE_R:
      return "bc1f"; // bc1f
      break;
    case IOP_BNE_R:
      return "bc1f"; // bc1f
      break;
    default:
      return "UNKNOWN";
      break;
  }
} //branch_instruction

bool CodeGenerator::marked(Symbol* sym) {
  if (sym->GetSymbolType() == ST_PARAMETER) {
    return true; // parameters are always marked (set by caller)
  }
  if (IsGlobal(sym)) {
    for (unsigned int i = 0; i < markGlobal.size(); i++) {
      // global sym already encountered in program
      if (markGlobal[i] == sym) {
        return true;
      }
    }
    markGlobal.push_back(sym);
  } else {
    for (unsigned int i = 0; i < mark.size(); i++) {
      // sym already encountered within scope
      if (mark[i] == sym) {
        return true;
      }
    }
    mark.push_back(sym);
  }
  return false;
} 

bool CodeGenerator::is_live(Symbol* sym) {
  IStatement* stmt;
  
  // we only check liveness for local variables
  if(IsGlobal(sym)) {
    return true;
  }
  // search symbol in following instructions
  for (unsigned int i = inst_count+1; i < icode->GetStatementCount(); i++) {
    stmt = icode->GetStatement(i);
    if (stmt->GetOperand1() != nullptr && stmt->GetOperand1()->GetOperandType() == OT_SYMBOL) {
      if (stmt->GetOperand1()->GetSymbol() == sym) {
        return true;
      }
    } else if (stmt->GetOperand2() != nullptr && stmt->GetOperand2()->GetOperandType() == OT_SYMBOL){
      if (stmt->GetOperand2()->GetSymbol() == sym) {
        return true;
      }
    } else if (stmt->GetResult() != nullptr) { // result always a symbol
      if (stmt->GetResult()->GetSymbol() == sym) {
        return true;
      }
    }
  }
  // symbol not found
  return false;
} //is_live

int CodeGenerator::find_reg_f(Symbol* sym) {
  for (int i = 0; i <= numRegs; i++) {
    if (float_regs[i] == sym && float_allocated[i] == true) {
      return i;
    }
  }
  return -1;
} //find_reg_f

int CodeGenerator::set_reg_f(Symbol* sym) {
  for (int i = fRegsBegin; i <= numRegs; i++) {
    if (!float_allocated[i] && i != syscallArg) {
      if (!labelWatch.empty() && sym != nullptr) {
        varWatch.push_back(sym);
      }
      float_regs[i] = sym;
      float_allocated[i] = true;
      return i;
    }
  }
  return -1;
} //set_reg_f

void CodeGenerator::write_f_to_memory(FILE* out) {
  int writes = 0;
  Symbol* sym;
  std::string dest;
  int i = fRegsBegin;
  // spill numWrites registers content to memory
  while (writes < numWrites) {
    i = rand()%(numRegs-fRegsBegin + 1) + fRegsBegin;
    if (i != syscallArg) {
      sym = float_regs[i];
      if (sym != nullptr && sym->GetSymbolType() != ST_TEMPVAR) {
        store(out, sym);
        deallocate_f(i);
      }
      writes++;
    }
  }
} //write_f_to_memory

int CodeGenerator::deallocate_f(int reg) {
  if (reg >= fRegsBegin && reg <= numRegs) {
    float_allocated[reg] = false;
    float_regs[reg] = nullptr;
    return 0;
  }
  return -1;
} //deallocate_f

int CodeGenerator::collect_f(FILE* out) {
  Symbol* temp;
  bool live = false;
  int freed = 0;
  
  // try to free registers containing symbols that are not live
  for (int i = fRegsBegin; i <= numRegs; i++) {
    temp = float_regs[i];
    if(float_allocated[i]){
      live = is_live(temp);
      if (!live) {
        deallocate_f(i);
        freed++;
      }
    }
    live = false;
  }
  if (freed > 0) {
    return freed;
  }
  // no registers freed, spill some to memory
  write_f_to_memory(out);
  return freed;
}

std::string CodeGenerator::allocate_f(FILE* out, Symbol* sym) {
  int reg = find_reg_f(sym);
  if (reg == -1) {
    //Not allocated
    if (sym->GetSymbolType() == ST_FUNCTION) {
      // function return value
      float_regs[0] = sym;
      float_allocated[0] = true;
      return "$f0";
    }
    reg = set_reg_f(sym);
    if (reg == -1) {
      //No register available, free space
      collect_f(out);
      reg = set_reg_f(sym);
      if (reg == -1) {
        printf("panik\n");
      }
      if (marked(sym)) {
        if (IsGlobal(sym)) {
          fprintf(out, "\tl.s\t$f%d, %s\n", reg, checkname(sym->GetName()).c_str()); 
        } else if (sym->GetSymbolType() == ST_VARIABLE || sym->GetSymbolType() == ST_PARAMETER) { 
          //symbol is local (variable or argument)
          fprintf(out, "\tl.s\t$f%d, ", reg);
          currentFrame.GenerateLocalAddress(out, sym);
          fprintf(out, "\n");
        }
      }
      return "$f" + std::to_string(reg);
    } else {
      //Allocated available register
      if (marked(sym)) {
        if (IsGlobal(sym)) {
          fprintf(out, "\tl.s\t$f%d, %s\n", reg, checkname(sym->GetName()).c_str()); 
        } else if (sym->GetSymbolType() == ST_VARIABLE || sym->GetSymbolType() == ST_PARAMETER) { 
          //symbol is local (variable or argument)
          fprintf(out, "\tl.s\t$f%d, ", reg);
          currentFrame.GenerateLocalAddress(out, sym);
          fprintf(out, "\n");
        }
      }
      return "$f" + std::to_string(reg);
    }
  } else {
    //Already allocated
    return "$f" + std::to_string(reg);
  }
  return "ERROR, no available register";
} //allocate_f

int CodeGenerator::find_reg_i(Symbol* sym) {
  for (int i = 0; i <= freeRegsEnd; i++) {
    if (int_regs[i] == sym && int_allocated[i] == true) {
      return i;
    }
  }
  return -1;
} //find_reg_i

int CodeGenerator::set_reg_i(Symbol* sym) {
  int i = freeRegsBegin;
  
  if (sym != nullptr && sym->GetSymbolType() == ST_PARAMETER) {
    i = argRegsBegin;
  }
  for (; i <= freeRegsEnd; i++) {
    if (!int_allocated[i]) {
      if (!labelWatch.empty() && sym != nullptr) {
        varWatch.push_back(sym);
      }
      int_regs[i] = sym;
      int_allocated[i] = true;
      return i;
    }
  }
  return -1;
} //set_reg_i

void CodeGenerator::write_i_to_memory(FILE* out) {
  int writes = 0;
  Symbol* sym;
  std::string dest;
  int i = freeRegsBegin;
  // spill numWrites registers content to memory
  while (writes < numWrites) {
    i = rand()%(freeRegsEnd-freeRegsBegin + 1) + freeRegsBegin;
    sym = int_regs[i];
    if(sym != nullptr && sym->GetSymbolType() != ST_TEMPVAR){
      store(out, sym);
      deallocate_i(i);
    }
    writes++;
  }
} //write_i_to_memory

void CodeGenerator::store(FILE* out, Symbol* sym) {
  std::string dest;
  bool integer = true;
  int i = find_reg_i(sym);
  
  if (sym->GetSymbolType() == ST_TEMPVAR) {
    return;
  }
  
  if (sym->GetReturnType() == RT_REAL) {
    integer = false;
    i = find_reg_f(sym);
  }
  if (i >= 0) {
    if (IsGlobal(sym)) {
      dest = checkname(sym->GetName());
    } else {
      dest = std::to_string(sym->GetOffset()) + "($fp)";
    }
    if (integer) {
      fprintf(out, "\tsw\t$%d, %s\n", i, dest.c_str());
    } else {
      fprintf(out, "\ts.s\t$f%d, %s\n", i, dest.c_str());
    }
  }
} //store

void CodeGenerator::deallocate_branch_regs(FILE* out, Symbol* sym) {
  // deallocate local integer variables
  for (int x = argRegsBegin; x <= freeRegsEnd; ++x) {
    if (int_allocated[x]) {
      for (unsigned int y = 0; y < varWatch.size(); ++y) {
        if (int_regs[x] == varWatch[y]) {
          if(is_live(int_regs[x])) {
            store(out, int_regs[x]);
          }
          deallocate_i(x);
        }
      }
    }
  }
  // deallocate local float variables
  for (int x = fRegsBegin; x <= numRegs; ++x) {
    if (float_allocated[x]) {
      for (unsigned int y = 0; y < varWatch.size(); ++y) {
        if (float_regs[x] == varWatch[y]) {
          if (is_live(float_regs[x])) {
            store(out, float_regs[x]);
          }
          deallocate_f(x);
        }
      }
    }
  }
  for (unsigned int i = 0; i < labelWatch.size(); ++i) {
    if (labelWatch[i] == sym) {
      labelWatch.erase(labelWatch.begin()+i);
    }
  }
  varWatch.clear();
} //deallocate_branch_regs

void CodeGenerator::deallocate_locals(FILE* out, SymbolTable* symtab, Symbol* funcSym) {
  Scope* s = symtab->GetScope(funcSym->GetName());
  Symbol* sym;
  
  // local variables  
  for (unsigned int i = 0; i < s->GetNumberOfSymbols(); i++) {
    sym = s->GetSymbol(i);
    if (sym->GetSymbolType() == ST_VARIABLE) {
      if (is_live(sym)) {
        store(out, sym);
      }
      if (sym->GetReturnType() == RT_INT) {
        deallocate_i(find_reg_i(sym));
      } else if (sym->GetReturnType() == RT_REAL) {
        deallocate_f(find_reg_f(sym));
      }
    }
  }
  // arguments
  for (int i = 0; i < funcSym->GetParameterCount(); i++) {
    sym = funcSym->GetParameter(i);
    if (is_live(sym)) {
        store(out, sym);
    }
    if (sym->GetReturnType() == RT_INT) {
      deallocate_i(find_reg_i(sym));
    } else if (sym->GetReturnType() == RT_REAL) {
      deallocate_f(find_reg_f(sym));
    }
  }
} //deallocate_locals

int CodeGenerator::deallocate_i(int reg) {
  if (reg >= argRegsBegin && reg <= freeRegsEnd) {
    int_allocated[reg] = false;
    int_regs[reg] = nullptr;
    return 0;
  }
  return -1;
} //deallocate_i

int CodeGenerator::collect_i(FILE* out) {
  Symbol* temp;
  //IStatement* stmt;
  bool live = false;
  int freed = 0;
  
  // try to free registers containing symbols that are not live
  for (int i = freeRegsBegin; i <= freeRegsEnd; i++) {
    temp = int_regs[i];
    if(int_allocated[i]){
      live = is_live(temp);
      if (!live) {
        deallocate_i(i);
        freed++;
      }
    }
    live = false;
  }
  if (freed > 0) {
    return freed;
  }
  // no registers freed, spill some to memory
  write_i_to_memory(out);
  return freed;
}

std::string CodeGenerator::allocate_i(FILE* out, Symbol* sym) {
  int reg = find_reg_i(sym);
  if (reg == -1) {
    //Not allocated
    if (sym->GetSymbolType() == ST_FUNCTION) {
      // function return value
      int_regs[2] = sym;
      int_allocated[2] = true;
      return "$2";
    }
    reg = set_reg_i(sym);
    if (reg == -1) {
      //No register available, free space
      collect_i(out);
      reg = set_reg_i(sym);
      if (reg == -1) {
        printf("panik\n");
      }
      if (marked(sym)) {
        if (IsGlobal(sym)) {
          fprintf(out, "\tlw\t$%d, %s\n", reg, checkname(sym->GetName()).c_str()); 
        } else if (sym->GetSymbolType() == ST_VARIABLE || sym->GetSymbolType() == ST_PARAMETER) { 
          //symbol is local (variable or argument)
          fprintf(out, "\tlw\t$%d, ", reg);
          currentFrame.GenerateLocalAddress(out, sym);
          fprintf(out, "\n");
        }
      }
      return "$" + std::to_string(reg);
    } else {
      //Allocated available register
      if (marked(sym)) {
        if (IsGlobal(sym)) {
          fprintf(out, "\tlw\t$%d, %s\n", reg, checkname(sym->GetName()).c_str()); 
        } else if (sym->GetSymbolType() == ST_VARIABLE || sym->GetSymbolType() == ST_PARAMETER) { 
          //symbol is local (variable or argument)
          fprintf(out, "\tlw\t$%d, ", reg);
          currentFrame.GenerateLocalAddress(out, sym);
          fprintf(out, "\n");
        }
      }
      return "$" + std::to_string(reg);
    }
  } else {
    //Already allocated
    return "$" + std::to_string(reg);
  }
  return "ERROR, no available register";
} //allocate_i

bool CodeGenerator::is_imm(IOperand* op) {
  if (op->GetOperandType() == OT_INT || op->GetOperandType() == OT_REAL) {
    return true;
  }
  return false;
} //is_imm

int CodeGenerator::imm_reg_f(FILE* out, float val) {
  int reg = set_reg_f(nullptr);
  if (reg == -1) {
    // no register free
    reg = collect_f(out);
    if (reg == 0) {
      // no registers freed, spill some to memory
      write_f_to_memory(out);
    }
    reg = set_reg_f(nullptr);
  }
  fprintf(out, "\tli.s\t$f%d, %f\n", reg, val);
  return reg;
} //imm_reg_f

int CodeGenerator::imm_reg_i(FILE* out, int val) {
  int reg = set_reg_i(nullptr);
  if (reg == -1) {
    // no register free
    reg = collect_i(out);
    if (reg == 0) {
      // no registers freed, spill some to memory
      write_i_to_memory(out);
    }
    reg = set_reg_i(nullptr);
  }
  fprintf(out, "\tli\t$%d, %d\n", reg, val);
  return reg;
} //imm_reg_i

void CodeGenerator::write_parameter(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  paramVec.push_back(stmt->GetOperand1());
}

void CodeGenerator::write_procedurecall(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  IOperand* param;
  int paramNum = 0;
  int immReg = -1;
  int Size = 4;
  Symbol* funcSym = stmt->GetOperand1()->GetSymbol();
  // Lower stack pointer
  if (funcSym->GetParameterCount() > 0) {
    fprintf(out, "\taddi\t$sp, $sp, %d\n", -(funcSym->GetParameterCount()*Size));
  }
  while (!paramVec.empty()) {
    // Place an argument
    param = paramVec.back();
    paramVec.pop_back();
    if (param->GetOperandType() == OT_SYMBOL) {
      if (param->GetSymbol()->GetReturnType() == RT_INT) {
        fprintf(out, "\tsw\t%s, %d($sp)\n", allocate_i(out, param->GetSymbol()).c_str(), paramNum*Size);
      } else if (param->GetSymbol()->GetReturnType() == RT_REAL) {
        fprintf(out, "\ts.s\t%s, %d($sp)\n", allocate_f(out, param->GetSymbol()).c_str(), paramNum*Size);
      } 
    } else if (param->GetOperandType() == OT_INT) {
      immReg = imm_reg_i(out, param->GetIntValue());
      fprintf(out, "\tsw\t$%d, %d($sp)\n", immReg, paramNum*Size);
      deallocate_i(immReg);
    } else {
      immReg = imm_reg_f(out, param->GetRealValue());
      fprintf(out, "\ts.s\t$f%d, %d($sp)\n", immReg, paramNum*Size);
      deallocate_f(immReg);
    }
    paramNum++;
  }
  
  // Deallocate parameters and local vars
  if (!(currentFrame.GetCurrentSubprogram()->GetSymbolType() == ST_PROGRAM)) {
    deallocate_locals(out, symtab, currentFrame.GetCurrentSubprogram());
  }
  
  // Jump to procedure
  fprintf(out, "\tjal\t%s\n", checkname(funcSym->GetName()).c_str());
} //write_procedurecall

void CodeGenerator::write_functioncall(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  IOperand* param;
  int paramNum = 0;
  int immReg = -1;
  int Size = 4;
  Symbol* funcSym = stmt->GetOperand1()->GetSymbol();
  
  // Lower stack pointer
  if (funcSym->GetParameterCount() > 0) {
    fprintf(out, "\taddi\t$sp, $sp, %d\n", -(funcSym->GetParameterCount()*Size));
  }
  while (!paramVec.empty()) {
    // Place an argument
    param = paramVec.back();
    paramVec.pop_back();
    if (param->GetOperandType() == OT_SYMBOL) {
      if (param->GetSymbol()->GetReturnType() == RT_INT) {
        fprintf(out, "\tsw\t%s, %d($sp)\n", allocate_i(out, param->GetSymbol()).c_str(), paramNum*Size);
      } else if (param->GetSymbol()->GetReturnType() == RT_REAL) {
        fprintf(out, "\ts.s\t%s, %d($sp)\n", allocate_f(out, param->GetSymbol()).c_str(), paramNum*Size);
      } 
    } else if (param->GetOperandType() == OT_INT) {
      immReg = imm_reg_i(out, param->GetIntValue());
      fprintf(out, "\tsw\t$%d, %d($sp)\n", immReg, paramNum*Size);
      deallocate_i(immReg);
    } else {
      immReg = imm_reg_f(out, param->GetRealValue());
      fprintf(out, "\ts.s\t$f%d, %d($sp)\n", immReg, paramNum*Size);
      deallocate_f(immReg);
    }
    paramNum++;
  }
  
  // Deallocate parameters and local vars
  if (!(currentFrame.GetCurrentSubprogram()->GetSymbolType() == ST_PROGRAM)) {
    deallocate_locals(out, symtab, currentFrame.GetCurrentSubprogram());
  }
  
  // Jump to function
  fprintf(out, "\tjal\t%s\n", checkname(funcSym->GetName()).c_str());
  
  // Save return value
  if (funcSym->GetReturnType() == RT_INT) {
    fprintf(out, "\tmove\t%s, $2\n", allocate_i(out, stmt->GetResult()->GetSymbol()).c_str());
    // Deallocate register 2
    int_allocated[2] = false;
    int_regs[2] = nullptr;
  } else if (funcSym->GetReturnType() == RT_REAL) {
    fprintf(out, "\tmov.s\t%s, $f0\n", allocate_f(out, stmt->GetResult()->GetSymbol()).c_str());
    // Deallocate float register 0
    float_allocated[0] = false;
    float_regs[0] = nullptr;
  }  
} //write_functioncall

void CodeGenerator::write_unary(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, op1, dest;
  bool op1_imm = is_imm(stmt->GetOperand1());
  Symbol* op1_sym;
  int imm_reg = -1;
  
  //Allocate operator, operands
  inst = GetInstructionName(stmt->GetOperator());
  dest = allocate_i(out, stmt->GetResult()->GetSymbol());

  if (!op1_imm) {
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_i(out, op1_sym);
  } else {
    imm_reg = imm_reg_i(out, stmt->GetOperand1()->GetIntValue());
    op1 = "$" + std::to_string(imm_reg);
  }
  
  fprintf(out, "\t%s\t%s, %s\n", inst.c_str(), dest.c_str(), op1.c_str());
  deallocate_i(imm_reg);
} //write_unary

void CodeGenerator::write_unary_f(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, op1, dest;
  bool op1_imm = is_imm(stmt->GetOperand1());
  Symbol* op1_sym;
  int imm_reg = -1;
  
  //Allocate operator, operands
  inst = GetInstructionName(stmt->GetOperator());
  dest = allocate_f(out, stmt->GetResult()->GetSymbol());

  if (!op1_imm) {
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_f(out, op1_sym);
  } else {
    imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
    op1 = "$f" + std::to_string(imm_reg);
  }
  
  fprintf(out, "\t%s\t%s, %s\n", inst.c_str(), dest.c_str(), op1.c_str());
  deallocate_f(imm_reg);
} //write_unary_f

void CodeGenerator::write_coercion(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, op1, dest;
  bool op1_imm = is_imm(stmt->GetOperand1());
  Symbol* op1_sym;
  int imm_reg = -1;
  
  //Allocate operator, operands
  inst = GetInstructionName(stmt->GetOperator());
  dest = allocate_f(out, stmt->GetResult()->GetSymbol());
  
  if (!op1_imm) {
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_i(out, op1_sym);
  } else {
    imm_reg = imm_reg_i(out, stmt->GetOperand1()->GetIntValue());
    op1 = "$" + std::to_string(imm_reg);
  }
  
  fprintf(out, "\t%s\t%s, %s\n", inst.c_str(), op1.c_str(), dest.c_str());
  fprintf(out, "\tcvt.s.w\t%s, %s\n", dest.c_str(), dest.c_str());
  deallocate_i(imm_reg);  
} //write_coercion

void CodeGenerator::write_jump(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  fprintf(out, "\tj\t%s\n", checkname(stmt->GetOperand1()->GetSymbol()->GetName()).c_str());
}  

void CodeGenerator::write_label(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  fprintf(out, "%s:\n", checkname(stmt->GetOperand1()->GetSymbol()->GetName()).c_str());
}

void CodeGenerator::write_assign_i(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, dest, op1;
  
  if (stmt->GetResult()->GetSymbol()->GetSymbolType() == ST_FUNCTION) {
    dest = "$2"; // function return value
  } else {
    dest = allocate_i(out, stmt->GetResult()->GetSymbol());
  }
  
  if (is_imm(stmt->GetOperand1())) { //do as immediate
    if (dest == "$2" || is_live(stmt->GetResult()->GetSymbol())) {
      fprintf(out, "\tli\t%s, %d\n", dest.c_str(), stmt->GetOperand1()->GetIntValue());
    } else {
      deallocate_i(find_reg_i(stmt->GetResult()->GetSymbol())); //deallocate result
    }
  } else { //do as symbol
    op1 = allocate_i(out, stmt->GetOperand1()->GetSymbol());
    fprintf(out, "\tmove\t%s, %s\n", dest.c_str(), op1.c_str()); 
  }
}

void CodeGenerator::write_assign_f(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, dest, op1;
  int imm_reg = -1;
  
  if (stmt->GetResult()->GetSymbol()->GetSymbolType() == ST_FUNCTION) {
    dest = "$f0"; // function return value
  } else {
    dest = allocate_f(out, stmt->GetResult()->GetSymbol());
  }
  
    if (is_imm(stmt->GetOperand1())) { //do as immediate
      if (dest == "f0" || is_live(stmt->GetResult()->GetSymbol())) {
        imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
        op1 = "$f" + std::to_string(imm_reg);
        fprintf(out, "\tmov.s\t%s, %s\n", dest.c_str(), op1.c_str());
        deallocate_f(imm_reg);
      } else {
        deallocate_f(find_reg_f(stmt->GetResult()->GetSymbol())); //deallocate result
      }
    } else { //do as symbol
      op1 = allocate_f(out, stmt->GetOperand1()->GetSymbol());
      fprintf(out, "\tmov.s\t%s, %s\n", dest.c_str(), op1.c_str());
      deallocate_f(imm_reg); 
    }
} //write_assign_f

void CodeGenerator::write_branch_i(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, label, op1, op2;
  bool op1_imm, op2_imm, zero = false;
  Symbol* op1_sym;
  Symbol* op2_sym;
  int imm_reg = -1;
  
  //Get instruction name
  inst = GetInstructionName(stmt->GetOperator());
  
  //Get operands
  label = stmt->GetResult()->GetSymbol()->GetName();
  
  op1_imm = is_imm(stmt->GetOperand1());
  op2_imm = is_imm(stmt->GetOperand2());
  
  if (!op1_imm && !op2_imm) { //src1 and src2 a reg
    op1 = allocate_i(out, stmt->GetOperand1()->GetSymbol());
    op2 = allocate_i(out, stmt->GetOperand2()->GetSymbol());
  } else if (!op1_imm && op2_imm) { //src1 reg, src2 imm
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_i(out, op1_sym);
    op2 =  std::to_string(stmt->GetOperand2()->GetIntValue());
    if (op2 == "0") {
      inst += "z";
      zero = true;
    }
  } else if (op1_imm && !op2_imm) { //src1 imm, src2 reg
    op2_sym = stmt->GetOperand2()->GetSymbol();
    op1 = allocate_i(out, op2_sym);
    op2 = std::to_string(stmt->GetOperand1()->GetIntValue());
    inst = ReverseBranch(stmt->GetOperator());
    if (op2 == "0") {
      inst += "z";
      zero = true;
    }
  } else if (op1_imm && op2_imm) { //src1 to register
    imm_reg = imm_reg_i(out, stmt->GetOperand1()->GetIntValue());
    op1 = "$" + std::to_string(imm_reg);
    op2 = std::to_string(stmt->GetOperand2()->GetIntValue());
    if (op2 == "0") {
      inst += "z";
      zero = true;
    }
  }  
  
  if (!zero) {
    fprintf(out, "\t%s\t%s, %s, %s\n", inst.c_str(), op1.c_str(), op2.c_str(), label.c_str());
  } else {
    fprintf(out, "\t%s\t%s, %s\n", inst.c_str(), op1.c_str(), label.c_str());
  }
  deallocate_i(imm_reg);
}

void CodeGenerator::write_branch_f(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, flag, label, op1, op2;
  bool op1_imm, op2_imm;
  Symbol* op1_sym;
  Symbol* op2_sym;
  int imm_reg = -1, imm_reg_2 = -1;
  
  // Get instruction name
  inst = GetInstructionName(stmt->GetOperator());
  
  // write compare
  op1_imm = is_imm(stmt->GetOperand1());
  op2_imm = is_imm(stmt->GetOperand2());
  
  if (!op1_imm && !op2_imm) { //src1 and src2 a reg
    op1 = allocate_f(out, stmt->GetOperand1()->GetSymbol());
    op2 = allocate_f(out, stmt->GetOperand2()->GetSymbol());
  } else if (!op1_imm && op2_imm) { //src1 reg, src2 imm
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_f(out, op1_sym);
    op2 =  std::to_string(stmt->GetOperand2()->GetRealValue());
  } else if (op1_imm && !op2_imm) { //src1 imm, src2 reg
    op2_sym = stmt->GetOperand2()->GetSymbol();
    imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
    op1 = "$f" + std::to_string(imm_reg);
    op2 = allocate_f(out, op2_sym);
  } else if (op1_imm && op2_imm) { //src1, src2 to register
    imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
    op1 = "$f" + std::to_string(imm_reg);
    imm_reg_2 = imm_reg_f(out, stmt->GetOperand2()->GetRealValue());
    op2 = "$f" + std::to_string(imm_reg_2);
  }  
  
  fprintf(out, "\t%s\t%s, %s\n", inst.c_str(), op1.c_str(), op2.c_str());
  deallocate_f(imm_reg);
  deallocate_f(imm_reg_2);
  
  // write branch
  flag = branch_instruction(stmt->GetOperator());
  label = stmt->GetResult()->GetSymbol()->GetName();
  fprintf(out, "\t%s\t%s\n", flag.c_str(), label.c_str());
  
} //write_branch_f

void CodeGenerator::write_arithmetic_i(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, dest, op1, op2;
  bool op1_imm, op2_imm;
  Symbol* op1_sym;
  Symbol* op2_sym;
  int imm_reg = -1;
  
  //get instruction name
  inst = GetInstructionName(stmt->GetOperator());
  
  //get operands
  dest = allocate_i(out, stmt->GetResult()->GetSymbol());
  
  op1_imm = is_imm(stmt->GetOperand1());
  op2_imm = is_imm(stmt->GetOperand2());
  if (!op1_imm && !op2_imm) { //src1 and src2 a reg
    op1 = allocate_i(out, stmt->GetOperand1()->GetSymbol());
    op2 = allocate_i(out, stmt->GetOperand2()->GetSymbol());
  } else if (!op1_imm && op2_imm) { //src1 reg, src2 imm
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_i(out, op1_sym);
    op2 =  std::to_string(stmt->GetOperand2()->GetIntValue());
    if (inst != "mul" && inst != "div" && inst != "sub") {
      inst += "i"; // added i for immediate instruction
    }
  } else if (op1_imm && !op2_imm) { //src1 imm, src2 reg
    op2_sym = stmt->GetOperand2()->GetSymbol();
    if (inst == "add" || inst == "mul" || inst == "or" || inst == "and") { //make it immediate
      op1 = allocate_i(out, op2_sym);
      op2 = std::to_string(stmt->GetOperand1()->GetIntValue());
      if (inst != "mul") {
        inst += "i"; // added i for immediate instruction
      }
    } else { //keep it normal
      imm_reg = imm_reg_i(out, stmt->GetOperand1()->GetIntValue());
      op1 = "$" + std::to_string(imm_reg);
      op2 = allocate_i(out, op2_sym);
    }
  } else if (op1_imm && op2_imm) { //src1 to register
    imm_reg = imm_reg_i(out, stmt->GetOperand1()->GetIntValue());
    op1 = "$" + std::to_string(imm_reg);
    op2 = std::to_string(stmt->GetOperand2()->GetIntValue());
    if (inst != "mul" && inst != "div" && inst != "sub") {
      inst += "i"; // added i for immediate instruction
    }
  }  
  
  fprintf(out, "\t%s\t%s, %s, %s\n", inst.c_str(), dest.c_str(), op1.c_str(), op2.c_str());
  deallocate_i(imm_reg);
} //write_arithmetic_i

void CodeGenerator::write_arithmetic_f(FILE* out, SymbolTable* symtab, IStatement* stmt) {
  std::string inst, dest, op1, op2;
  bool op1_imm, op2_imm;
  Symbol* op1_sym;
  Symbol* op2_sym;
  int imm_reg = -1, imm_reg_2 = -1;
  
  //get instruction name
  inst = GetInstructionName(stmt->GetOperator());
  
  //get operands
  dest = allocate_f(out, stmt->GetResult()->GetSymbol());
  
  op1_imm = is_imm(stmt->GetOperand1());
  op2_imm = is_imm(stmt->GetOperand2());
  
  if (!op1_imm && !op2_imm) { //src1 and src2 a reg
    op1 = allocate_f(out, stmt->GetOperand1()->GetSymbol());
    op2 = allocate_f(out, stmt->GetOperand2()->GetSymbol());
  } else if (!op1_imm && op2_imm) { //src1 reg, src2 imm
    op1_sym = stmt->GetOperand1()->GetSymbol();
    op1 = allocate_f(out, op1_sym);
    imm_reg = imm_reg_f(out, stmt->GetOperand2()->GetRealValue());
    op2 = "$f" + std::to_string(imm_reg);
  } else if (op1_imm && !op2_imm) { //src1 imm, src2 reg
    op2_sym = stmt->GetOperand2()->GetSymbol();
    imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
    op1 = "$f" + std::to_string(imm_reg);
    op2 = allocate_f(out, op2_sym);
  } else if (op1_imm && op2_imm) { //src1, src2 to register
    imm_reg = imm_reg_f(out, stmt->GetOperand1()->GetRealValue());
    op1 = "$f" + std::to_string(imm_reg);
    imm_reg_2 = imm_reg_f(out, stmt->GetOperand2()->GetRealValue());
    op2 = "$f" + std::to_string(imm_reg_2);
  }  
  
  fprintf(out, "\t%s\t%s, %s, %s\n", inst.c_str(), dest.c_str(), op1.c_str(), op2.c_str());
  deallocate_f(imm_reg);
  deallocate_f(imm_reg_2);
} //write_arithmetic_f

// Generates a header
void CodeGenerator::GenerateHeader(FILE * out) {
  fprintf(out, "## Output generated by ./comp\n");
  fprintf(out, "##\n");
  fprintf(out, "\n");
}


// Generates the declarations for the global variables
void CodeGenerator::GenerateGlobalDecls(FILE * out, SymbolTable * symtab) {
  Scope * globalScope;
  Symbol * sym;

  fprintf(out, "\t.data\n");
  fprintf(out, "\t.align\t4\n");
  fprintf(out, "\t# Global declarations should appear here\n");

  globalScope = symtab->GetRootScope();

  // Here we iterate over all global variables in the symbol table and
  // generate a declaration.
  for (unsigned int i = 0; i < globalScope->GetNumberOfSymbols(); i++) {
    sym = globalScope->GetSymbol(i);
    if (IsGlobal(sym)) {        
      switch(sym->GetReturnType()) {
        case RT_INT:
          fprintf(out, "\t%s:\t.word 0\n", checkname(sym->GetName()).c_str());
          break;
        case RT_REAL:
          fprintf(out, "\t%s:\t.float 0.0\n", checkname(sym->GetName()).c_str());
          break;
        default:
          break;
      }
    }
  }

  fprintf(out, "\n");
}    

// Takes an IntermediateCode object and emits MIPS assembly instructions
void CodeGenerator::GenerateCode(FILE * out, SymbolTable * symtab, IntermediateCode * inputCode) {
  IStatement * stmt;
  
  srand(time(NULL));
  icode = inputCode;

  // Generate a general 'stub'.
  fprintf(out, "\t.text\n");
  fprintf(out, "\t.align\t4\n");
  fprintf(out, "\t.globl\tmain\n");
  fprintf(out, "\t.ent\tmain\n");
  fprintf(out, "main:\n");
  fprintf(out, "\tjal\t%s\n", inputCode->GetProgramName().c_str());
  fprintf(out, "\tadd\t$v0,$0,10\n");
  fprintf(out, "\tsyscall\n");
  fprintf(out, "\t.end\tmain\n");
  fprintf(out, "\n");

  // Here we step through the intermediate code and output assembly instructions
  for (unsigned int i = 0; i < inputCode->GetStatementCount(); i++) {
    stmt = inputCode->GetStatement(i);
    
    switch (stmt->GetOperator()) {
      case IOP_SUBPROG:
        mark.clear();
        // The start of a subprogram: analyze it, and generate the prologue.
        currentFrame.AnalyzeSubprogram(symtab, stmt->GetOperand1()->GetSymbol());
        currentFrame.GeneratePrologue(out);
        break;
      
      case IOP_RETURN:
      case IOP_RETURN_I:
      case IOP_RETURN_R:
        // Deallocate parameters and local vars
        if (!(currentFrame.GetCurrentSubprogram()->GetSymbolType() == ST_PROGRAM)) {
          deallocate_locals(out, symtab, currentFrame.GetCurrentSubprogram());
        }
        // The end of a subprogram: generate the epilogue
        currentFrame.GenerateEpilogue(out);
        break;
      
      case IOP_PARAM_I:
      case IOP_PARAM_R:
        write_parameter(out, symtab, stmt);
        break;
      
      case IOP_PROCCALL:
        write_procedurecall(out, symtab, stmt);
        break;
      case IOP_FUNCCALL:
        write_functioncall(out, symtab, stmt);
        break;

      case IOP_LABEL:
        deallocate_branch_regs(out, stmt->GetOperand1()->GetSymbol());
        write_label(out, symtab, stmt);
        break;
      case IOP_GOTO:
        write_jump(out, symtab, stmt);
        labelWatch.push_back(stmt->GetOperand1()->GetSymbol());
        break;

      case IOP_ASSIGN_I:
        write_assign_i(out, symtab, stmt);
        break;
      case IOP_ASSIGN_R:
        write_assign_f(out, symtab, stmt);
        break;

      case IOP_BEQ_I:
      case IOP_BLT_I:
      case IOP_BGT_I:
      case IOP_BLE_I:
      case IOP_BGE_I:
      case IOP_BNE_I:
        write_branch_i(out, symtab, stmt);
        labelWatch.push_back(stmt->GetResult()->GetSymbol());
        break;
      case IOP_BEQ_R:
      case IOP_BLT_R:
      case IOP_BGT_R:
      case IOP_BLE_R:
      case IOP_BGE_R:
      case IOP_BNE_R:
        write_branch_f(out, symtab, stmt);
        labelWatch.push_back(stmt->GetResult()->GetSymbol());
        break;
      
      case IOP_ADD_I:
      case IOP_SUB_I:
      case IOP_MUL_I:
      case IOP_DIV_I:
      case IOP_MOD:
      case IOP_AND:
      case IOP_OR:
        write_arithmetic_i(out, symtab, stmt);
        break;
        
      case IOP_ADD_R:
      case IOP_SUB_R:
      case IOP_MUL_R:
      case IOP_DIV_R:
        write_arithmetic_f(out, symtab, stmt);
        break;
        
      case IOP_NOT:
      case IOP_UNARY_MINUS_I:
        write_unary(out, symtab, stmt);
        break;
      case IOP_UNARY_MINUS_R:
        write_unary_f(out, symtab, stmt);
        break;
      case IOP_INT_TO_REAL:
        write_coercion(out, symtab, stmt);
        break;
      default:
        fprintf(stderr,
                "[CodeGenerator::GenerateCode()] Unhandled intermediate operator '%s'\n",
                IOperatorToString(stmt->GetOperator())
               );
        break;
    }
    
    inst_count++;
  }

  fprintf(out, "\n");
}


// Generates a trailer
void CodeGenerator::GenerateTrailer(FILE * out) {
  fprintf(out, "# Here you can insert the standard functions (readinteger etc.)\n");
  fprintf(out, "\n");
  
  // writeinteger
  fprintf(out, "\t.ent\twriteinteger\n");
  fprintf(out, "writeinteger:\n");
  //prologue
  fprintf(out, "\tlw\t$a0, ($sp)\n");
  fprintf(out, "\taddi\t$sp, $sp, -8\n");
  fprintf(out, "\tsw\t$ra, 4($sp)\n");
  fprintf(out, "\tsw\t$fp, ($sp)\n");
  fprintf(out, "\taddi\t$fp, $sp, 4\n");
  //system call
  fprintf(out, "\tadd\t$v0, $0, 1\n");
  fprintf(out, "\tsyscall\n"); 
  //epilogue
  fprintf(out, "\tlw\t$ra, ($fp)\n");
  fprintf(out, "\taddi\t$sp, $fp, 8\n");
  fprintf(out, "\tlw\t$fp, -4($fp)\n");
  fprintf(out, "\tjr\t$ra\n");
  fprintf(out, "\t.end\twriteinteger\n");
  fprintf(out, "\n");
  
  // writereal
  fprintf(out, "\t.ent\twritereal\n");
  fprintf(out, "writereal:\n");
  //prologue
  fprintf(out, "\tl.s\t$f12, ($sp)\n");
  fprintf(out, "\taddi\t$sp, $sp, -8\n");
  fprintf(out, "\tsw\t$ra, 4($sp)\n");
  fprintf(out, "\tsw\t$fp, ($sp)\n");
  fprintf(out, "\taddi\t$fp, $sp, 4\n");
  //system call
  fprintf(out, "\tadd\t$v0, $0, 2\n");
  fprintf(out, "\tsyscall\n"); 
  //epilogue
  fprintf(out, "\tlw\t$ra, ($fp)\n");
  fprintf(out, "\taddi\t$sp, $fp, 8\n");
  fprintf(out, "\tlw\t$fp, -4($fp)\n");
  fprintf(out, "\tjr\t$ra\n");
  fprintf(out, "\t.end\twritereal\n");
  fprintf(out, "\n");
  
  //readinteger
  fprintf(out, "\t.ent\treadinteger\n");
  fprintf(out, "readinteger:\n");
  //prologue
  fprintf(out, "\taddi\t$sp, $sp, -8\n");
  fprintf(out, "\tsw\t$ra, 4($sp)\n");
  fprintf(out, "\tsw\t$fp, ($sp)\n");
  fprintf(out, "\taddi\t$fp, $sp, 4\n");
  //system cal
  fprintf(out, "\tadd\t$v0, $0, 5\n");
  fprintf(out, "\tsyscall\n");
  //epilogue
  fprintf(out, "\tlw\t$ra, ($fp)\n");
  fprintf(out, "\taddi\t$sp, $fp, 4\n");
  fprintf(out, "\tlw\t$fp, -4($fp)\n");
  fprintf(out, "\tjr\t$ra\n");
  fprintf(out, "\t.end\treadinteger\n");
  fprintf(out, "\n");
  
  //readreal
  fprintf(out, "\t.ent\treadreal\n");
  fprintf(out, "readreal:\n");
  //prologue
  fprintf(out, "\taddi\t$sp, $sp, -8\n");
  fprintf(out, "\tsw\t$ra, 4($sp)\n");
  fprintf(out, "\tsw\t$fp, ($sp)\n");
  fprintf(out, "\taddi\t$fp, $sp, 4\n");
  //system cal
  fprintf(out, "\tadd\t$v0, $0, 6\n");
  fprintf(out, "\tsyscall\n");
  //epilogue
  fprintf(out, "\tlw\t$ra, ($fp)\n");
  fprintf(out, "\taddi\t$sp, $fp, 4\n");
  fprintf(out, "\tlw\t$fp, -4($fp)\n");
  fprintf(out, "\tjr\t$ra\n");
  fprintf(out, "\t.end\treadreal\n");
  fprintf(out, "\n");
}

