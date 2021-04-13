/*
 * StackFrameManager.cc - Implementation of the StackFrameManager class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#include "StackFrameManager.h"

// Constructor:
StackFrameManager::StackFrameManager() {
  currentSubprogram = NULL;
  currentVarSize = 0;
}


// Destructor:
StackFrameManager::~StackFrameManager() {
}

Symbol* StackFrameManager::GetCurrentSubprogram() {
  return currentSubprogram;
}

// Analyzes a subprogram and computes the stack frame layout.
// This function should always be called once for a subprogram before
// calls to GeneratePrologue/Epilogue or GenerateLocalAddress are made.
void StackFrameManager::AnalyzeSubprogram(SymbolTable * symtab, Symbol * subprogram) {
  bool isProgramBody = false;
  int varCount = 0; // in amount
  int varOffset = -8; // offset local variables
  int argOffset = 4; // offset arguments
  
  Scope* s = symtab->GetScope(subprogram->GetName());
  Symbol* sym;
  
  currentSubprogram = subprogram;

  if (subprogram->GetSymbolType() == ST_PROGRAM) {
    // AnalyzeSubprogram is also called for the program body of the input
    // program. However, this program body is somewhat different compared
    // to regular functions and procedures: it has no parameters, and its
    // variables are already declared in the global scope. Therefore, the
    // following boolean is provided, such that you can skip certain steps.
    isProgramBody = true;
  }
  
  // Here you should count the number of local variables and decide at which
  // offset inside the stack frame (relative to register $fp) each local
  // variable will be placed. The same thing has to be done for each parameter
  // of the subprogram. For functions, you can consider to reserve an
  // additional field that can be used to store the return value.
  // You can use the SetOffset() method of a Symbol in order to assign
  // offsets to symbols.
  // Also, computing the total size of the stack frame might be useful.
  
  if (!isProgramBody) {
    // optional return value
    if (subprogram->GetSymbolType() == ST_FUNCTION) {
      subprogram->SetOffset(varOffset);
      varOffset -= varSize;
      varCount++;
    }  
    // local variables  
    for (unsigned int i = 0; i < s->GetNumberOfSymbols(); i++) {
      sym = s->GetSymbol(i);
      if (sym->GetSymbolType() == ST_VARIABLE) {
        sym->SetOffset(varOffset);
        varOffset -= varSize;
        varCount++;
      }
    }
    currentVarSize = varCount * varSize;
    // arguments
    for (int i = 0; i < subprogram->GetParameterCount(); i++) {
      sym = subprogram->GetParameter(i);
      sym->SetOffset(argOffset);
      argOffset += varSize;
    }
  } 
}


// Generates the code that sets up the stack frame at the entry point of a
// subprogram.
void StackFrameManager::GeneratePrologue(FILE * out) {
  // Indicate the beginning of a new subprogram:
  fprintf(out, "\t.ent\t%s\n", currentSubprogram->GetName().c_str());
  fprintf(out, "%s:\n", currentSubprogram->GetName().c_str());
  
  // Here you should generate code that uses the data computed in
  // AnalyzeSubprogram(). Basically, this means:
  //  - Saving the return address on the stack
  //  - Saving the old frame pointer on the stack
  //  - Setting the new frame pointer value
  //  - Setting the new stack pointer value
  
  fprintf(out, "\taddi\t$sp, $sp, -%d\n", 8 + currentVarSize);
  fprintf(out, "\tsw\t$ra, %d($sp)\n", 4 + currentVarSize);
  fprintf(out, "\tsw\t$fp, %d($sp)\n", currentVarSize);
  fprintf(out, "\taddi\t$fp, $sp, %d\n", 4 + currentVarSize);  
}


// Generates the code that discards the stack frame at the leaving point of
// a subprogram.
void StackFrameManager::GenerateEpilogue(FILE * out) {
  bool isProgramBody = false;
  int currentParamSize = 0;
  
  if (currentSubprogram->GetSymbolType() == ST_PROGRAM) {
    isProgramBody = true;
  }
  
  if (!isProgramBody) {
    currentParamSize = (currentSubprogram->GetParameterCount()) * 4;
  }
  
  // Here you should generate code that uses the data computed in
  // AnalyzeSubprogram(). Basically, this means:
  //  - Restoring the return address
  //  - Restoring the old frame pointer
  //  - Setting the new stack pointer value
  //  - Jumping back to the return address
  
  fprintf(out, "\tlw\t$ra, ($fp)\n");
  fprintf(out, "\taddi\t$sp, $fp, %d\n", currentParamSize+4);   
  fprintf(out, "\tlw\t$fp, -4($fp)\n");
  fprintf(out, "\tjr\t$ra\n");
  fprintf(out, "\t.end\t%s\n", currentSubprogram->GetName().c_str());
  fprintf(out, "\n");
}


// Generates a $fp-relative address for a local variable or parameter.
// The output can be used directly as an operand for load/store instructions.
void StackFrameManager::GenerateLocalAddress(FILE * out, Symbol * sym) {
  int offset = 0;
  
  // Here you should generate code that uses the data computed in
  // AnalyzeSubprogram(). When this function is invoked, AnalyzeSubprogram()
  // should have computed an offset relative to register $fp for each local
  // variable and parameter of the current subprogram.
  
  offset = sym->GetOffset();
  
  // Now write the address to the output file:
  fprintf(out, "%d($fp)", offset);
}
