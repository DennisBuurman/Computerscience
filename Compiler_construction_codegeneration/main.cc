/*
 * main.cc
 * 
 * This file contains some generic functions. It is part of
 * the assignment for the Compiler Construction course at the Leiden Institute
 * of Advanced Computer Science (LIACS), Leiden University.
 */

#include <cstring>
#include "SymbolTable.h"
#include "ICGenerator.h"
#include "CodeGenerator.h"

/* Globals */
ICGenerator * icg;
IntermediateCode * icode;
CodeGenerator * cg;
bool bWarnings = true;
bool bOptimize1 = false;


/* Extern functions */
int yyparse();                // The parser
int yylex_destroy();          // Destroy the parser
bool IsGlobal(Symbol * sym);  // Is the specified symbol a global variable?


/* Extern variables */
extern int lineno;
extern SyntaxTree * tree;
extern SymbolTable * symtab;
extern int gErrorCount;
extern int gWarningCount;




void PrintError (const char * msg) {
  fprintf (stderr, "Error on line %d: %s\n", lineno, msg);
  gErrorCount++;
}//PrintError


void PrintWarning (const char * msg) {
  if (bWarnings) {
    fprintf (stderr, "Warning on line %d: %s\n", lineno, msg);
    gWarningCount++;
  }
}//PrintWarning


/* Parse the command line */
int ParseCommandline(int argc, char ** argv)
{
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-w") == 0) {
      bWarnings = false;
    }
    else if (strcmp(argv[i], "-O0") == 0) {
      bOptimize1 = false;
    }
    else if (strcmp(argv[i], "-O1") == 0) {
      bOptimize1 = true;
    }
    else {
      fprintf(stderr, "Error: unrecognized command line option '%s'\n", argv[i]);
      return 1;
    }
  }

  return 0;
}


void initialize()
{
  /* Create & initialize the abstract data structures */
  tree = new SyntaxTree;
  symtab = new SymbolTable;
  icg = new ICGenerator;
  cg = new CodeGenerator;
  icode = NULL;

  gErrorCount = 0;
  gWarningCount = 0;
}


void cleanup()
{
  /* Free objects */
  delete cg;
  delete icg;
  delete tree;
  delete symtab;
  if (icode) delete icode;
  yylex_destroy();
}


int main(int argc, char ** argv)
{
  FILE * outfile;
  const char * outfilename = "out.s";
  int parseResult;
  
  if (ParseCommandline(argc, argv) == 1) {
    return 1;
  }

  initialize();
  
  /* Start the parsing procedure */
  parseResult = yyparse();
  
  /* Dump the syntax tree & symbol table */
  tree->Dump (stdout);
  printf("\n");
  symtab->Dump (stdout);

  /* Generate intermediate code, if everything is still ok */
  if (!(parseResult || gErrorCount)) {
    if (bOptimize1) {
      icg->Preprocess(tree, symtab);
    }
    icode = icg->GenerateIntermediateCode(tree, symtab);
    icode->Dump(stdout);
    if (bOptimize1) {
      icg->Postprocess(icode, symtab);
    }

    /* Now generate the final assembly */
    outfile = fopen(outfilename, "w+");
    cg->GenerateHeader(outfile);
    cg->GenerateGlobalDecls(outfile, symtab);
    cg->GenerateCode(outfile, symtab, icode);
    cg->GenerateTrailer(outfile);
    fclose(outfile);
    printf("Output has been written to \"%s\".\n", outfilename);
  }

  printf("%d error(s), %d warning(s)\n", gErrorCount + parseResult, gWarningCount);
  
  cleanup();
  
  return (parseResult || gErrorCount);

}
