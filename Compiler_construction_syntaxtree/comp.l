/* C declarations */
%{
 
#include <stdio.h>
#include <string>
#include "debug.h"
#include "Node.h"
#include "y.tab.h"
#include "types.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif


int lineno = 1;

%}

%option nounput

LETTER  [a-zA-Z]
DIGIT   [0-9]
ID      {LETTER}({LETTER}|{DIGIT})*
DIGITS  {DIGIT}{DIGIT}*

OPTIONAL_FRACTION       ("."{DIGITS})?
OPTIONAL_EXPONENT       (("E"("+"|"-")?{DIGITS}))?
NUM     {DIGITS}{OPTIONAL_FRACTION}{OPTIONAL_EXPONENT} 


/* Patterns */
%%

{NUM}           {
                  pmesg(90, "Lexer: NUM: %s\n", yytext);
                  yylval.numStr = strdup(yytext);
                  return NUM;
                }

program         {
                  pmesg(90, "Lexer: PROGRAM: %s\n", yytext);
                  return PROGRAM;
                }

if              {
                  pmesg(90, "Lexer: IF: %s\n", yytext);
                  return IF;
                }

then            {
                  pmesg(90, "Lexer: THEN: %s\n", yytext);
                  return THEN;
                }

else            {
                  pmesg(90, "Lexer: ELSE: %s\n", yytext);
                  return ELSE;
                }

while           {
                  pmesg(90, "Lexer: WHILE: %s\n", yytext);
                  return WHILE;
                }

do              {
                  pmesg(90, "Lexer: DO: %s\n", yytext);
                  return DO;
                }

begin           {
                  pmesg(90, "Lexer: PASCALBEGIN: %s\n", yytext);
                  return PASCALBEGIN;
                }

end             {
                  pmesg(90, "Lexer: END: %s\n", yytext);
                  return END;
                }

procedure       {
                  pmesg(90, "Lexer: PROCEDURE: %s\n", yytext);
                  return PROCEDURE;
                }

function        {
                  pmesg(90, "Lexer: FUNCTION: %s\n", yytext);
                  return FUNCTION;
                }

var             {
                  pmesg(90, "Lexer: VAR: %s\n", yytext);
                  return VAR;
                }

integer         {
                  pmesg(90, "Lexer: INTEGER: %s\n", yytext);
                  return INTEGER;
                }

real            {
                  pmesg(90, "Lexer: REAL: %s\n", yytext);
                  return REAL;
                }
":="            {
                  pmesg(90, "Lexer: ASSIGNOP: %s\n", yytext);
                  return ASSIGNOP;
                }

"="|"<"|">"|"<="|">="|"<>"      {
                  pmesg(90, "Lexer: RELOP: %s\n", yytext);
                  yylval.idStr = strdup(yytext);
                  return RELOP;
                }

"or"            { /* "-" cannot be returned as ADDOP, since it can also 
                     be UMINUS; we can't distinguish these at the lex 
                     level */
                  pmesg(90, "Lexer: ADDOP: %s\n", yytext);
                  yylval.idStr = strdup(yytext);
                  return ADDOP;
                }

"*"|"/"|"div"|"mod"|"and"   {
                pmesg(90, "Lexer: MULOP: %s\n", yytext);
                yylval.idStr = strdup(yytext);
                return MULOP;
                }

"not"           {
                  pmesg(90, "Lexer: NOT: %s\n", yytext);
                  return NOT;
                }

{ID}            { 
                  pmesg(90, "Lexer: ID: %s\n", yytext);
                  yylval.idStr = strdup(yytext);
                  return ID;
                }

"{"[^{}]*"}"    { /* Eat comments */ 
                  int cnt;
                  pmesg(90, "Lexer: Comment eaten\n");
                  for(cnt = 0; yytext[cnt] != '\0'; cnt++) {
                    if (yytext[cnt] == '\n') {
                      lineno++;
                    }
                  }
                  /* no return */
                }

\n              {
                  lineno++;
                  pmesg(90, "Lexer: lineno == %i\n", lineno);
                  /* no return */
                }

[ \t]+          {  /* eat up whitespace */
                  /* no return */
                }

.               {
                  pmesg(90, "Lexer: Single non-identifier character: %s\n", yytext ); 
                  return(yytext[0]);
                }

%%
