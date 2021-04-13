/*
 * ICGenerator.cc - Implementation of the ICGenerator class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#include <sstream>
#include "ICGenerator.h"

// Constructor
ICGenerator::ICGenerator() {
  tempvarIndex = 0;
  labelIndex = 0;
  symtab = NULL;
  tree = NULL;
}


// Destructor
ICGenerator::~ICGenerator() {
}


// Preprocesses the syntax tree; this method is called before
// GenerateIntermediateCode() if optimizations are enabled
void ICGenerator::Preprocess (SyntaxTree * tree, SymbolTable * symtab) {
  this->symtab = symtab;
  this->tree = tree;
}



// Postprocesses the intermediate code; this method is called after
// GenerateIntermediateCode() if optimizations are enabled
void ICGenerator::Postprocess (IntermediateCode * code, SymbolTable * symtab) {
  this->symtab = symtab;
}
