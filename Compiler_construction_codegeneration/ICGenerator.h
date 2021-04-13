/*
 * ICGenerator.h - Declaration of the ICGenerator class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#ifndef _ICGENERATOR_H_
#define _ICGENERATOR_H_

#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "IntermediateCode.h"


// This class handles the intermediate code generation. Extend it to your own
// needs. Do not remove any of the predefined methods or members.
class ICGenerator {
  public:
    // Constructor
    ICGenerator();

    // Destructor
    ~ICGenerator();

    // Preprocesses the syntax tree; this method is called before
    // GenerateIntermediateCode() if optimizations are enabled
    void Preprocess (SyntaxTree * tree, SymbolTable * symtab);

    // Takes a SyntaxTree and converts it into an IntermediateCode structure
    IntermediateCode * GenerateIntermediateCode (SyntaxTree * inputTree, SymbolTable * symtab);

    // Postprocesses the intermediate code; this method is called after
    // GenerateIntermediateCode() if optimizations are enabled
    void Postprocess (IntermediateCode * code, SymbolTable * symtab);

  private:

    // Internal functions
    IOperand*ProcessTree(IntermediateCode*,Node*);
    void ProcessParamlist(IntermediateCode*,Node*);
    IStatement*ProcessBoolExpr(IntermediateCode*,Node*,Symbol*);
    void ReverseBranchCondition(IStatement*);
    Symbol*GenerateTempVar(ReturnType);
    Symbol*GenerateLabelSymbol();
    IStatement*GenerateLabel(IntermediateCode*,Symbol*);

    // Internal variables
    unsigned int tempvarIndex;
    unsigned int labelIndex;

    // Pointer to the symbol table
    SymbolTable * symtab;

    // Pointer to the tree
    SyntaxTree * tree;

    // <insert your own private members and methods>

};

#endif
