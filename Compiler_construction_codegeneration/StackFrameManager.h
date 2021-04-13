/*
 * StackFrameManager.h - Declaration of the StackFrameManager class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#ifndef _STACKFRAMEMANAGER_H_
#define _STACKFRAMEMANAGER_H_

#include <cstdio>
#include "SymbolTable.h"

#include <vector>

// This class gathers information about the stack frame (also known as
// an activation record) of a subprogram and provides methods to generate
// code based on the gathered information.
class StackFrameManager {
  public:
    // Constructor:
    StackFrameManager();
  
    // Destructor:
    ~StackFrameManager();
    
    // Analyzes a subprogram and computes the stack frame layout.
    // This function should always be called once for a subprogram before
    // calls to GeneratePrologue/Epilogue or GenerateLocalAddress are made.
    void AnalyzeSubprogram(SymbolTable * symtab, Symbol * subprogram);
    
    // Generates the code that sets up the stack frame at the entry point of a
    // subprogram.
    void GeneratePrologue(FILE * out);
    
    // Generates the code that discards the stack frame at the leaving point of
    // a subprogram.
    void GenerateEpilogue(FILE * out);
    
    // Generates a $fp-relative address for a local variable or parameter.
    // The output can be used directly as an operand for load/store instructions.
    void GenerateLocalAddress(FILE * out, Symbol * sym);
    
    // ... your own public members and methods ...
    Symbol* GetCurrentSubprogram();
    
  private:

    // The current subprogram
    Symbol * currentSubprogram;

    // ... your own private members and methods ...
    // Total size of all local subprogram variables, in bytes
    int currentVarSize;
    // Size of a variable, in bytes 
    int varSize = 4;
    
};


#endif
