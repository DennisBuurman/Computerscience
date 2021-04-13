/* C declarations */
%{

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "debug.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "Node.h"

/* Set verbosity */
bool verbose = true; //Verbose unless otherwise specified

/* Type precedences (used for coercion) 
   sorted by ascending precedence. */

vector<ReturnType> precedenceVec{
                                 RT_ERROR,
                                 RT_INT,
                                 RT_REAL
                                };
                                  

/* Prototypes */
static void yyerror(const char *);
int check_parameter_type(Symbol* subprogram, int, Node*, ReturnType &);
int coercion(ReturnType, ReturnType, ReturnType &);
void predefine();
void add_symbol_to_vec(string, SymbolType, int);
int add_scope_to_symtab(string, SymbolType, int);
ReturnType determine_returntype(string);
int check_nullsymb(Symbol* &, string);

/* Import from comp.l */
#ifdef __cplusplus
extern "C" {
#endif

int yylex(void);   /* Lexer function */

#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number */


/* Other declarations */
SyntaxTree* tree;                       /* The syntax tree */
SymbolTable* symtab;                    /* The symbol table */
string mainScopeName;                   /* The name of the main scope */
vector<Symbol*> s;                      /* Temporary symbol storage */
Symbol_Subprogram* subsimp;             /* Temporary subprogram symbol */
vector<string> ScopeStack;              /* Stack for scopes */
vector<pair<Symbol*, int>> paramcount;  /* Parameter counter*/
int errCount = 0;
int warnCount = 0;

#ifdef DEBUG
int msglevel = 90; /* higher = more debugging messages */
#else
int msglevel = 0;   /* don't bother sensitive souls */
#endif

%}

/* Start symbol */
%start program

/* Tokens */
%token PROGRAM ID VAR NUM INTEGER REAL FUNCTION PROCEDURE 
%token PASCALBEGIN /* BEGIN is used internally by flex :-( */
%token END ADDOP ASSIGNOP IF THEN ELSE WHILE DO RELOP
%token MULOP NOT PROG_PARAMETER IRRELEVANT
%token UMINUS /* unary minus */

/* Types to pass between lexer, rules and actions */
%union {
  char *idStr;
  char *numStr;
  Node *node;
}

%expect 1  /* shift/reduce conflict: dangling ELSE */
%%       
         
/* GRAMMAR RULES AND ACTIONS */

program:        PROGRAM ID {
                  /* The program name has just been read; save it */
                  mainScopeName = $<idStr>2;

                  /* Free the chars allocated by the lexer */
                  free($<idStr>2);
                  
                  /* Put the program name into the syntax tree */
                  tree->SetProgramName(mainScopeName);
                  
                  /* Create a new scope in the symbol table */
                  symtab->AddScope(mainScopeName);
                  ScopeStack.push_back($<idStr>2);
                  
                  // Predefine read/write integer/real
                  predefine();
                }
                ';' declarations subprogram_declarations 
                compound_statement '.'{
                  tree->SetProgramBody($<node>7);
                  if (errCount > 0) {
                   YYERROR;
                  }
                }
                ;


type_identifier_list:
                ',' ID {
                  //Add ID to vector, to match with type later
                  add_symbol_to_vec($<idStr>2, s.back()->GetSymbolType(), lineno);
                }
                type_identifier_list 

                | ':' type {
                  ReturnType ret = determine_returntype($<idStr>2);
                  Symbol* temp;
                  string err;
                  
                  //Match previously added IDs from vector to type and add to scope
                  for (std::vector<Symbol*>::iterator it = s.begin(); it != s.end(); it++) { 
                    (*it)->SetReturnType(ret);
                    if (symtab->AddSymbol(*it) == -1) {
                      //Detect double definitions of the symbol name
                      err = "error: double definition of: ";
                      err += (*it)->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      if ((temp = symtab->GetSymbol((*it)->GetName())) != nullptr) {
                        temp->SetSymbolType(ST_ERROR);
                      } 
                    }
                    if ((*it)->GetSymbolType() == ST_PARAMETER) {
                      //Add parameter to appropriate symbol, if needed
                      if (subsimp) {
                        subsimp->AddParameter(*it); 
                      }
                    }
                  }
                  //Clear the vector for new declarations
                  s.clear();
                }
                ; 


declarations:   /* In declarations the VAR-variables are listed for the 
                   program as well as for the procedures and functions. */

                declarations VAR ID {
                  //Add ID to a vector, to be matched with type later
                  add_symbol_to_vec($<idStr>3, ST_VARIABLE, lineno);
                }
                
                type_identifier_list ';'

                | /* lambda */
                ;


type:           standard_type

                ;


standard_type:  INTEGER {
                  strcpy($<idStr>$, "int");
                }

                | REAL {
                  strcpy($<idStr>$, "real");
                }
                ; 


subprogram_declarations:
                subprogram_declarations subprogram_declaration ';'
                | /* lambda */
                ;


subprogram_declaration:
                subprogram_head declarations compound_statement {
                  //After processing function declaration, add it to syntax tree and reset scope
                  string tempstr = symtab->GetCurrentScopeName();
                  symtab->SetCurrentScope(ScopeStack.back());
                  ScopeStack.pop_back();
                  tree->AddSubprogram(tempstr, $<node>3);
                };


subprogram_head: 
                FUNCTION ID {
                  int r;
                  string err;
                  
                  //Create scope for the new function
                  if ((r = add_scope_to_symtab($<idStr>2, ST_FUNCTION, lineno)) < 0) {
                    err = "error: double definition of: ";
                    err += $<idStr>2;
                    if (r == -1) {
                      yyerror(err.c_str());
                      errCount++;
                      
                    } else if (r == -2) {
                      yyerror(err.c_str());
                      errCount++;
                    }
                  }
                }
                arguments ':' standard_type ';' {
                  ReturnType ret;
                  
                  //Set function return type
                  if ((ret = determine_returntype($<idStr>5)) == RT_ERROR) {
                    string err = "error: return type error: ";
                    err += $<idStr>5;
                    yyerror(err.c_str());
                    errCount++;
                  }
                  subsimp->SetReturnType(ret);
                }

                | PROCEDURE ID {
                  int r;
                  
                  //Create a scope for the procedure
                  if ((r = add_scope_to_symtab($<idStr>2, ST_PROCEDURE, lineno)) < 0) {
                    string err = "error: double definiton of: ";
                    err += $<idStr>2;
                    if (r == -1) {
                      yyerror(err.c_str());
                      errCount++;
                    } else if (r == -2) {
                      yyerror(err.c_str());
                      errCount++;
                    }
                  }
                }
                arguments ';'
                ;


arguments:      '(' parameter_lists ')'
                | /* lambda */
                ;


parameter_lists: parameter_list ';' parameter_lists
                
                | parameter_list 
                ;


parameter_list: ID {
                  //Add ID to vector, to be matched with type later
                  add_symbol_to_vec($<idStr>1, ST_PARAMETER, lineno);
                }
                type_identifier_list
                ;


compound_statement:
                PASCALBEGIN optional_statements END {
                  $<node>$ = $<node>2;
                }
                ;


optional_statements:
                statement_list

                | /* lambda */
                ;


//EXAMPLE
statement_list: statement {
                  $<node>$ = tree->CreateParentNode (NODE_STATEMENT_LIST,
                               RT_VOID, $<node>1, tree->CreateLeaf());
                }

                | statement_list ';' statement {
                  $<node>$ = tree->CreateParentNode (NODE_STATEMENT_LIST,
                               RT_VOID, $<node>1, $<node>3);
                }
                ;


statement:      variable ASSIGNOP expression {
                  ReturnType ret;
                  string err;
                  
                  //Determine if coercion is needed and/or possible
                  switch (coercion($<node>1->GetReturnType(), $<node>3->GetReturnType(), ret)) {
                    case 0: //No coercion needed
                      $<node>$ = tree->CreateParentNode(NODE_ASSIGNMENT, ret, $<node>1, $<node>3);
                    break;
                    case 1: //Coercion needed on left node (not possible)
                      yyerror("error: coercion not possible");
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, $<node>3);
                    break;
                    case 2: //Coercion needed on right node (possible)
                      yyerror("warning: coercion performed");
                      warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ASSIGNMENT, ret, $<node>1,
                                                        tree->CreateParentNode(NODE_COERCION, ret, $<node>3));
                    break;
                    default:
                    break;
                  }
                }
                
                | procedure_statement {
                  $<node>$ = $<node>1;
                }
                
                | compound_statement {
                  $<node>$ = $<node>1;
                }
                
                | IF boollikeexp THEN statement optional_else_statement {
                  $<node>$ = tree->CreateParentNode(NODE_IF, RT_VOID,
                                                    tree->CreateParentNode(NODE_BOOLEXPR, RT_BOOL, $<node>2),
                                                    tree->CreateParentNode(NODE_IF_TARGETS, RT_VOID,
                                                    $<node>4, $<node>5));
                }
                
                | WHILE boollikeexp DO statement {
                  $<node>$ = tree->CreateParentNode(NODE_WHILE, RT_VOID, 
                                                    tree->CreateParentNode(NODE_BOOLEXPR, RT_BOOL, $<node>2),
                                                    $<node>4);
                }
                ;


optional_else_statement:
                ELSE statement {
                  $<node>$ = $<node>2;
                }

                | 
                {
                  $<node>$ = tree->CreateLeaf();
                }
                ;


variable:
                ID {
                  Symbol* simp = symtab->GetSymbol($<idStr>1);
                  string err;
                  
                  //Add variable to syntax tree, if allowed
                  if (check_nullsymb(simp, $<idStr>1)) {
                    //Check if variable has been declared
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                  } else {
                    //Check if variable has been declared as an error
                    if (simp->GetSymbolType() == ST_ERROR) {
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                    } else if ((simp->GetSymbolType() == ST_FUNCTION || simp->GetSymbolType() == ST_PROCEDURE)
                    && simp->GetName() != symtab->GetCurrentScopeName()) {
                      //Check if subprogram is being used as a variable
                      err = "error: cannot assign value to subprogram ";
                      err += simp->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                    } else {
                      //Everything OK, add variable to syntax tree
                      $<node>$ = tree->CreateLeaf(simp);
                    }
                  }
                }
                
                ;


procedure_statement:
                ID {
                  Symbol* simp = symtab->GetSymbol($<idStr>1);
                  string err;
                  
                  //Add procedure call to syntax tree, if allowed
                  if (check_nullsymb(simp, $<idStr>1)) {
                    //Check if procedure is declared
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                  } else {
                    if (simp->GetSymbolType() == ST_ERROR) {
                      //Check if procedure was declared wrongly 
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), tree->CreateLeaf());
                    } else if (simp->GetSymbolType() != ST_PROCEDURE) {
                      //Check if a function is being called as a procedure
                      err = "error: result discarded for function ";
                      err += simp->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), tree->CreateLeaf());
                    } else { 
                      //Everything OK, add procedure call to syntax tree
                      $<node>$ = tree->CreateParentNode(NODE_PROCCALL, RT_VOID,
                                                        tree->CreateLeaf(simp), tree->CreateLeaf());
                    }
                  }
                }
                
                | ID {
                  //TODO
                  if (symtab->GetSymbol($<idStr>1) != nullptr) {
                    paramcount.push_back(make_pair(symtab->GetSymbol($<idStr>1), 0));
                  }
                }
                '(' expression_list ')' {
                  Symbol* simp = symtab->GetSymbol($<idStr>1);
                  string err;
                  
                  //Add procedure to syntax tree (if allowed), this time with an argument list
                  if (check_nullsymb(simp, $<idStr>1)) {
                    //Check if procedure has been declared
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                  } else if (simp->GetSymbolType() != ST_PROCEDURE) {
                      //Check if function is being used as a procedure
                      err = "error: result discarded for function ";
                      err += simp->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), $<node>4);
                  } else {
                    if (simp->GetSymbolType() == ST_ERROR) {
                      //Check if procedure was declared wrongly
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), $<node>4);
                    } else {
                      //Everything OK, add procedure to syntax tree
                      $<node>$ = tree->CreateParentNode(NODE_PROCCALL, RT_VOID, tree->CreateLeaf(simp), $<node>4);
                    }
                  }
                }
                
                ;


expression_list:
                expression {
                  int p;
                  int pc;
                  
                  //Check if function/procedure call contains right amount and type of arguments
                  if (paramcount.empty()) {
                    yyerror("Invalid function/procedure call");
                    errCount++;
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1);
                  } else {
                    paramcount.back().second++;
                    p = paramcount.back().first->GetParameterCount();
                    pc = paramcount.back().second;
                    ReturnType retTemp;
                    string err;
                    if (p > pc) {
                      //Too few parameters counted for function/procedure call
                      err = "error: too few arguments on: ";
                      err += paramcount.back().first->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1);
                    } else if (p < pc) {
                      //Too many parameters counted for function/procedure call
                      err = "error: too many arguments on: ";
                      err += paramcount.back().first->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, tree->CreateLeaf());
                    } else {
                      //Correct amount of parameters: Check parameter type
                      switch (check_parameter_type(paramcount.back().first, paramcount.back().second, $<node>1, retTemp)) {
                        case 0: //Types match, no coercion
                          $<node>$ = tree->CreateParentNode (NODE_EXPRLIST, RT_VOID, $<node>1, tree->CreateLeaf());
                        break;
                        case 1: //Coercion needed and possible on the expression
                          err = "warning: coercion performed";
                          yyerror(err.c_str());
                          warnCount++;
                          $<node>$ = tree->CreateParentNode(NODE_COERCION, retTemp, $<node>1); //TODO nested coercion
                        break;
                        case -1: //Coercion needed but not possible on the expression
                          err = "error: coercion could not be performed";
                          yyerror(err.c_str());
                          errCount++;
                          $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1);
                        break;
                      }
                    }
                    paramcount.pop_back();
                  }
                }
                
                | expression {
                  ReturnType retTemp;
                  paramcount.back().second++;
                  string err;
                  
                  //TODO
                  switch (check_parameter_type(paramcount.back().first, paramcount.back().second, $<node>1, retTemp)) {
                    case 0: //Types match, no coercion
                      $<node>$ = tree->CreateParentNode (NODE_EXPRLIST, RT_VOID, $<node>1, tree->CreateLeaf());
                    break;
                    case 1: //Coercion needed on the expression and possible
                      err = "warning: coercion performed";
                      yyerror(err.c_str());
                      warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_COERCION, retTemp, $<node>1);
                    break;
                    case -1: //Coercion needed on the expression but not possible
                      err = "error: coercion could not be performed";
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1);
                    break;
                  }
                } ',' expression_list {
                  $<node>$ = tree->CreateParentNode (NODE_EXPRLIST,
                                                     RT_VOID, $<node>1, $<node>4); 
                }
                ;

boollikeexp:    expression RELOP expression {
                  NodeType nt;
                  ReturnType ret;
                  bool error = false;
                  
                  //Determine the operator
                  if (strcmp($<idStr>2, "=") == 0) {
                    nt = NODE_REL_EQUAL;
                  } else if (strcmp($<idStr>2, "<") == 0) {
                    nt = NODE_REL_LT;
                  } else if (strcmp($<idStr>2, ">") == 0) {
                    nt = NODE_REL_GT;
                  } else if (strcmp($<idStr>2, "<=") == 0) {
                    nt = NODE_REL_LTE;
                  } else if (strcmp($<idStr>2, ">=") == 0) {
                    nt = NODE_REL_GTE;
                  } else if (strcmp($<idStr>2, "<>") == 0) {
                    nt = NODE_REL_NOTEQUAL;
                  } else {
                    //Throw an error if the operator does not exist
                    string err = "error: invalid operator: ";
                    err += $<idStr>2;
                    yyerror(err.c_str());
                    errCount++;
                    error = true;
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf());
                  }
                  
                  //Proceed if no error has been found
                  if (!error) {
                    switch (coercion($<node>1->GetReturnType(), $<node>3->GetReturnType(), ret)) {
                      case 0: //No coercion needed
                        $<node>$ = tree->CreateParentNode(nt, ret, $<node>1, $<node>3);
                      break;
                      case 1: //Coercion needed in left hand of expression
                        yyerror("Warning: Coercion in left hand of expression");
                        warnCount++;
                        $<node>$ = tree->CreateParentNode(nt, ret, 
                                                          tree->CreateParentNode(NODE_COERCION, ret, $<node>1), 
                                                          $<node>3);
                      break;
                      case 2: //Coercion needed in right hand of expression
                        yyerror("Warning: Coercion in right hand of expression");
                        warnCount++;
                        $<node>$ = tree->CreateParentNode(nt, ret, $<node>1,
                                                          tree->CreateParentNode(NODE_COERCION, ret, $<node>3));
                      break;
                      default:
                      break;
                    }
                  }
                }
                
                | '(' boollikeexp ')' {
                  $<node>$ = $<node>2;
                }
                ; /* to allow stuff like "if (a > 3) then" */


expression:     simple_expression 
                ;


simple_expression:
                term
                
                | signplus term {
                  ReturnType ret = $<node>2->GetReturnType();
                  $<node>$ = tree->CreateParentNode(NODE_SIGNPLUS, ret, $<node>2);
                }
        
                | signminus term {
                  ReturnType ret = $<node>2->GetReturnType();
                  $<node>$ = tree->CreateParentNode(NODE_SIGNMINUS, ret, $<node>2);
                }
        
                | simple_expression '+' term {
                  ReturnType ret;
                  
                  //Determine if coercion is needed
                  switch (coercion($<node>1->GetReturnType(), $<node>3->GetReturnType(), ret)) {
                    case 0: //No coercion needed
                      $<node>$ = tree->CreateParentNode(NODE_ADD, ret, $<node>1, $<node>3);
                    break;
                    case 1: //Coercion needed in left side of expression
                      yyerror("Warning: Coercion in left side of expression");
                       warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ADD, ret,
                                                        tree->CreateParentNode(NODE_COERCION, ret, $<node>1),
                                                        $<node>3);
                    break;
                    case 2: //Coercion needed in right side of expression
                      yyerror("Warning: Coercion in right side of expression");
                      warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ADD, ret, $<node>1,
                                                        tree->CreateParentNode(NODE_COERCION, ret, $<node>3));
                    break;
                    default:
                    break;
                  }
                }
                
                | simple_expression '-' term {
                  ReturnType ret;
                  
                  //Determine if coercion is needed
                  switch (coercion($<node>1->GetReturnType(), $<node>3->GetReturnType(), ret)) {
                    case 0: //No coercion needed
                      $<node>$ = tree->CreateParentNode(NODE_SUB, ret, $<node>1, $<node>3);
                    break;
                    case 1: //Coercion needed in left side of expression
                      yyerror("Warning: Coercion in left side of expression");
                      warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_SUB, ret,
                                                        tree->CreateParentNode(NODE_COERCION, ret, $<node>1),
                                                        $<node>3);
                    break;
                    case 2: //Coercion needed in right side of the expression
                      yyerror("Warning: Coercion in right side of expression");
                      warnCount++;
                      $<node>$ = tree->CreateParentNode(NODE_SUB, ret, $<node>1,
                                                        tree->CreateParentNode(NODE_COERCION, ret, $<node>3));
                    break;
                    default:
                    break;
                  }
                }
                
                | simple_expression ADDOP term {
                  //Determine if operands are of type integer and if so, add expression to tree
                  if (!($<node>1->GetReturnType() == RT_INT && $<node>3->GetReturnType() == RT_INT)) {
                    yyerror("error: attempted OR operation with non-integers");
                    errCount++;
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, $<node>3);
                  } else {
                    $<node>$ = tree->CreateParentNode(NODE_OR, RT_INT, $<node>1, $<node>3);
                  }
                }
                ;


term:           factor
                
                | term MULOP factor {
                  NodeType nt;
                  ReturnType ret;
                  bool error = false;
                  string err;
                   
                  //Determine operator type
                  if (!strcmp($<idStr>2, "*")) {
                    nt = NODE_MUL;
                  } else if (!strcmp($<idStr>2, "/")) {
                    nt = NODE_DIV;
                  } else if (!strcmp($<idStr>2, "div")) {
                    //Determine if operands are of type integer and if so, add expression to tree
                    if (!($<node>1->GetReturnType() == RT_INT && $<node>3->GetReturnType() == RT_INT)) {
                      yyerror("error: attempted integer division with non-integers");
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, $<node>3);
                      error = true;
                    } else {
                      nt = NODE_IDIV;
                    }
                  } else if (!strcmp($<idStr>2, "mod")) {
                    nt = NODE_MOD;
                  } else if (!strcmp($<idStr>2, "and")) {
                    //Determine if operands are of type integer and if so, add expression to tree
                    if (!($<node>1->GetReturnType() == RT_INT && $<node>3->GetReturnType() == RT_INT)) {
                      yyerror("error: attempted AND operation with non-integers");
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, $<node>3);
                      error = true;
                    } else {
                      nt = NODE_AND;
                    }
                  } else {
                    //Operator not found, handle accordingly
                    err = "error: invalid operator: ";
                    err += $<idStr>2;
                    yyerror(err.c_str());
                    errCount++;
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>1, $<node>3);
                    error = true;
                  }
                  
                  if (!error) {
                    switch (coercion($<node>1->GetReturnType(), $<node>3->GetReturnType(), ret)) {
                      case 0:
                        $<node>$ = tree->CreateParentNode(nt, ret, $<node>1, $<node>3);
                      break;
                      case 1:
                        yyerror("Warning: Coercion in left hand of expression");
                        warnCount++;
                        $<node>$ = tree->CreateParentNode(nt, ret, 
                                                          tree->CreateParentNode(NODE_COERCION, ret, $<node>1), 
                                                          $<node>3);
                      break;
                      case 2:
                        yyerror("Warning: Coercion in right hand of expression");
                        warnCount++;
                        $<node>$ = tree->CreateParentNode(nt, ret, $<node>1,
                                                          tree->CreateParentNode(NODE_COERCION, ret, $<node>3));
                      break;
                      default:
                      break;
                    }
                  }
                }

                ;

factor:         ID {
                  Symbol* simp = symtab->GetSymbol($<idStr>1);
                  SymbolType st;
                  string err;
                  if (check_nullsymb(simp, $<idStr>1)) {
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                  } else {
                    st = simp->GetSymbolType();
                    if (st == ST_ERROR) {
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                    } else if (st == ST_PROCEDURE) {
                      err = "error: attempting to use return value for procedure ";
                      err += simp->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                    } else if (st == ST_FUNCTION) {
                      $<node>$ = tree->CreateParentNode(NODE_FUNCTIONCALL, simp->GetReturnType(), 
                                                        tree->CreateLeaf(simp), tree->CreateLeaf());
                    } else {
                      $<node>$ = tree->CreateLeaf(symtab->GetSymbol($<idStr>1));
                    }
                  }
                }
                
                | ID {
                  if (symtab->GetSymbol($<idStr>1) != nullptr)
                    paramcount.push_back(make_pair(symtab->GetSymbol($<idStr>1) , 0));
                }
                '(' expression_list ')' {
                  Symbol* simp = symtab->GetSymbol($<idStr>1);
                  SymbolType st;
                  string err;
                  if (check_nullsymb(simp, $<idStr>1)) {
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp));
                  } else {
                    st = simp->GetSymbolType();
                    if (st == ST_PROCEDURE) {
                      err = "error: attempting to use return value for procedure ";
                      err += simp->GetName();
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), $<node>4);
                    } else if (st == ST_FUNCTION) {
                      $<node>$ = tree->CreateParentNode(NODE_FUNCTIONCALL, simp->GetReturnType(), tree->CreateLeaf(simp), $<node>4);
                    } else {
                      string err = "error: symbol not a function/procedure: ";
                      err += $<idStr>1;
                      yyerror(err.c_str());
                      errCount++;
                      $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, tree->CreateLeaf(simp), $<node>4);
                    }
                  }
                }
                | NUM {
                  if (strchr($<idStr>1, '.')) {
                    $<node>$ = tree->CreateLeaf((float)atof($<idStr>1));
                  } else {
                    $<node>$ = tree->CreateLeaf(atoi($<idStr>1));
                  }
                }
                
                | '(' expression ')' {
                  $<node>$ = $<node>2;
                }
                
                | NOT factor {
                  if (!($<node>2->GetReturnType() == RT_INT)) {
                    yyerror("error: attempting NOT operation with non-integer");
                    errCount++;
                    $<node>$ = tree->CreateParentNode(NODE_ERROR, RT_ERROR, $<node>2);
                  } else {
                    ReturnType ret = $<node>2->GetReturnType();
                    $<node>$ = tree->CreateParentNode(NODE_NOT, ret, $<node>2);
                  }
                }
                
                ;


signplus:       '+'
                ;
                
signminus:      '-' %prec UMINUS
                ;

%%

/* End of rules, more C code will follow now */
int find_precedence(ReturnType ret) {
  for (unsigned int i = 0; i < precedenceVec.size(); ++i) {
    if (precedenceVec[i] == ret) {
      return i;
    }
  }
  return -1; //not found
} //find_precedence
  
//Coercion system can be expanded by adding types to precedenceVec
int coercion(ReturnType returnLeft, ReturnType returnRight, ReturnType &ret) {
  int prec_left;
  int prec_right;
  
  //Determine type precedence
  if ((prec_left = find_precedence(returnLeft)) == -1) {
    yyerror("error: attempting to coerce unknown type");
    errCount++;
    return 0;
  }
  if ((prec_right = find_precedence(returnRight)) == -1) {
    yyerror("error: attempting to coerce unknown type");
    errCount++;
    return 0;
  }
  
  //Determine required coercion
  if (returnLeft == returnRight) {
    //types match, no coercion needed
    ret = returnLeft;
    return 0;
  } else if (prec_left < prec_right) {
    //coercion needed on left node
    ret = returnRight;
    return 1;
  } else if (prec_left > prec_right) {
    //coercion needed on right node
    ret = returnLeft;
    return 2;
  }
  
  return 0;
} //coercion

int check_parameter_type(Symbol* subprogram, int paramno, Node* expression, ReturnType &ret) {
  if (!subprogram->GetParameter(paramno-1)) {
    return 0; //Too many arguments, but this will be handled somewhere else
  }
  ReturnType return_expected = subprogram->GetParameter(paramno-1)->GetReturnType();
  ReturnType return_actual = expression->GetReturnType();
  //Determine if coercions are needed and perform if possible
  switch (coercion(return_actual, return_expected, ret)) {
    case 0: //Return types match, no coercion needed
      //no stress
      return 0;
    break;
    case 1: //Coercion on the expression (actual_return) is needed and possible
      //Passing 'ret'
      return 1;
    break;
      case 2: //Coercion on the expression (actual_return) is needed but not possible
      return -1;
    break;
    default: 
    break;
  }
  return -2;
} //check_parameter_type

void add_symbol_to_vec(string name, SymbolType st, int line) {
  Symbol* new_symbol = new Symbol;
  new_symbol->SetName(name);
  new_symbol->SetSymbolType(st);
  new_symbol->SetLine(line); 
  s.push_back(new_symbol);
} //add_symbol_to_vec

int add_scope_to_symtab(string name, SymbolType st, int line) {
  Symbol* temp;
  subsimp = new Symbol_Subprogram;
  subsimp->SetSymbolType(st);
  subsimp->SetName(name);
  subsimp->SetLine(line);
  if ((symtab->AddSymbol(subsimp) == -1)) {
    //Adding symbol to symtab failed (duplicate)
    if ((temp = symtab->GetSymbol(name)) != nullptr) {
      temp->SetSymbolType(ST_ERROR);
    } 
    return -1;
  }
  ScopeStack.push_back(symtab->GetCurrentScopeName());
  if (symtab->AddScope(name) == nullptr) {
    return -2;
  }
  return 0;
}

ReturnType determine_returntype(string type) {
  if (type == "int") {
    return RT_INT;
  } else if (type == "real") {
    return RT_REAL;
  }
  return RT_ERROR;
}

//Check if a symbol reference points to null and handle accordingly
int check_nullsymb(Symbol* &symb, string name) {
  string errMsg;
  if (symb) {
    //This is not a null symbol; return accordingly
    return 0;
  } else {
    //Create error message and count error
    errMsg = "error: undeclared symbol \'" + name + "\'";
    yyerror(errMsg.c_str());
    errCount++;
    
    //Add error symbol to symbol table
    symb = new Symbol;
    symb->SetName(name);
    symb->SetSymbolType(ST_ERROR);
    symb->SetReturnType(RT_ERROR);
    symb->SetLine(lineno);
    symtab->AddSymbol(symb);
    
    //This is a null symbol; return accordingly
    return 1;
  }
} 

void initialize() {
  /* Create & initialize the abstract data structures */
  tree = new SyntaxTree;
  symtab = new SymbolTable;
}

void predefine() {
  Symbol_Subprogram* readint = new Symbol_Subprogram;
  Symbol_Subprogram* readreal = new Symbol_Subprogram;
  Symbol_Subprogram* writeint = new Symbol_Subprogram;
  Symbol_Subprogram* writereal = new Symbol_Subprogram;
  
  // function readinteger
  readint->SetName("readinteger");
  readint->SetSymbolType(ST_FUNCTION);
  readint->SetReturnType(RT_INT);
  
  // function readreal
  readreal->SetName("readreal");
  readreal->SetSymbolType(ST_FUNCTION);
  readreal->SetReturnType(RT_REAL);
  
  // procedure writeinteger
  writeint->SetName("writeinteger");
  writeint->SetSymbolType(ST_PROCEDURE);
  writeint->SetReturnType(RT_VOID);
  Symbol* simp = new Symbol;
  simp->SetName("a");
  simp->SetSymbolType(ST_PARAMETER);
  simp->SetReturnType(RT_INT);
  writeint->AddParameter(simp);
  
  // procedure writereal
  writereal->SetName("writereal");
  writereal->SetSymbolType(ST_PROCEDURE);
  writereal->SetReturnType(RT_VOID);
  simp = new Symbol; 
  simp->SetName("ToWrite");
  simp->SetSymbolType(ST_PARAMETER);
  simp->SetReturnType(RT_REAL);
  writereal->AddParameter(simp);
  
  // add to symbol table
  symtab->AddSymbol(readint);
  symtab->AddSymbol(readreal);
  symtab->AddSymbol(writeint);
  symtab->AddSymbol(writereal);
}

void cleanup() {
  /* Free objects */
  delete tree;
  delete symtab; 
}


int main(int argc, char* argv[]) {
  int parseResult;
  
  //Determine if verbosity should be turned off
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-s")) {
      verbose = false;
    }
  }
  
  initialize();

  /* Start the parsing procedure */
  parseResult = yyparse();
  
  /* Dump the syntax tree & symbol table */
  tree->Dump(stdout);
  printf("\n");
  symtab->Dump(stdout);
  
  cleanup(); 
     
  //Print amount of errors and warnings found in the program   
  printf("Amount of errors: %d\n", errCount);
  printf("Amount of warnings: %d\n", warnCount);
 
  /* Return the exit code. Note that the exit code does not depend on
     parseResult only, but also on the result of your semantic checks */  
  printf("Parseresult is: %d\n", parseResult);
  return parseResult;
}


static void yyerror(const char *s) {
  if (verbose) {
    fprintf(stderr, "line %d: %s\n", lineno, s);
  }
}

int yywrap() {
  return(1);
}
