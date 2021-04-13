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

#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <string>
#include <vector>

// This class handles the intermediate code generation. Extend it to your own
// needs.
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
    // counter used to uniquely name tempVars
    unsigned int tempCount = 1;
    
    // counter used to uniquely name labels
    unsigned int labelCount = 1;

    //Stack containing while-loop labels that still need to be jumped to
    std::vector<Symbol*> vecGotoWhile;
    
    //Stack containing end of control sequence labels that
    //needed to be added to the icode
    std::vector<Symbol*> vecLabelEnd;
    
    //Stack containing the jumps to the end of the control
    //sequence needed before the else-statement
    std::vector<Symbol*> vecGotoEnd;
    
    //Stack containing the labels to else-statements
    std::vector<Symbol*> vecLabelElse;
  
    // Generates a temporary symbol
    Symbol * GenerateTempVar(ReturnType type);
    
    // Generates a Symbol for a label
    Symbol * GenerateLabel();
    
    // Takes a node and checks if it is a unary type
    bool is_unary(Node* node);
    
    // Takes a node and checks if it is an arithmetic node
    bool is_arithmetic(Node* node);
    
    // Replaces root (child of parent) with node
    void replace_child(Node* node, Node* root, Node* parent);
    
    // Adds last label element of labelVec as IOP_LABEL instruction to icode
    inline void label_statement(IntermediateCode* icode);
    
    // Returns IOperand based on type of node; nullptr if unvalid node
    IOperand* determine_operand(Node* node);
    
    // Adds a tempVar with symbol temp to the tree that replaces root
    void add_tempvar_to_tree(Node* root, Node* parent, Symbol* temp);
    
    // replaces root with a zero node: integer zero if bool integer is true, else real zero
    void make_node_zero (bool integer, Node* root, Node* parent);
    
    // checks if node child contains zero value: bool integer used to determine integer or real type
    bool is_zero(Node* child, bool &integer);
    
    // checks if node is a function or procedure call
    bool is_call(Node* node);
    
    // makes root a zero node on multiplication by zero
    // returns true on succes, else false 
    bool check_node_zeroproduct (Node* root, Node* parent);
    
    // replaces root with an identity node on multiplication by one or addition with zero
    // bool replaceleft notes the child that replaces root; true -> replace with left, else replace with right
    bool make_node_identity(bool replaceleft, Node* root, Node* parent);
    
    // checks if node child contains value one
    bool is_one(Node* child);
    
    // changes root if identity is encountered (addition by zero or multiplication by one)
    // true on succes, else false
    bool check_node_identity(Node* root, Node* parent);
    
    // performs a constant fold on an integer (sub)expression and replaces root with a new node containing the result
    void make_node_folding(int op1, int op2, Node* root, Node* parent);
    
    // performs a constant fold on a real (sub)expression and replaces root with a new node containing the result
    void make_node_folding(float op1, float op2, Node* root, Node* parent);
    
    // replaces root with the result of constant folding it's expression if possible
    // true on succes, else false
    bool check_node_folding(Node* root, Node* parent);
    
    // tries to perform one of the possible optimizations on root node
    void optimize_node(Node* root, Node* parent); 
    
    // recursively walks through the tree starting from root
    // tries to perform optimizations on the nodes in post order (first children, then yourself)
    void preprocessTree(Node* root, Node* parent);
    
    // adds IOP_PARAM and IOP_FUNCCALL statements to icode
    void convert_functioncall(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab);
    
    // adds IOP_PARAM and IOP_PROCCALL statements to icode
    void convert_proccall(Node* root, IntermediateCode* icode);
    
    // sets labels and jumps
    void convert_boolexpr(Node* root, Node* parent, IntermediateCode* icode);
    
    // sets branching instruction
    void convert_relational(Node* root, IntermediateCode* icode, Symbol* labelBegin);
    
    // adds an assignment statement to the icode; also handles function call return value assignments
    void convert_assignment(Node* root, IntermediateCode* icode);
    
    // adds a new temporary variable to the tree and assigns the subexpression result to the temporary variable
    void convert_arithmetic(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab);
    
    // adds a new temporary variable to the tree and assigns the result of the unary operation to the temporary variable
    void convert_unary(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab);

    // adds labels and jumps to if and while
    void convert_if_targets(Node* root, IntermediateCode* icode);
    void convert_if(Node* root, IntermediateCode* icode);
    void convert_while(Node* root, IntermediateCode* icode);
    bool fold_signbit(Node* node, Node* parent);
    
    // recursively handles the nodes in post order walk starting at root
    // calls a convert function or sets a label according to the NodeType of the root node
    void recurseTree(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab);
};

#endif
