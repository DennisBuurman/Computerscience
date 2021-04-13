/* C declarations */
%{
 
#include <stdio.h>
#include "y.tab.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);

#if defined(__cplusplus)
}
#endif


int c;
extern int yylval;

%}

%option nounput
%option noinput


/* Patterns */
%%

" "       ;
[0-9]     {
            c = yytext[0];
            yylval = c - '0';
            return DIGIT;
          }

[^0-9\b]  {
            c = yytext[0];
            return c;
          }

[*]{2}    {
            // Detects concatenation of two * symbols and returns power token
            c = yytext[0];
            return POWER;
          }

"PI"      {
            // Detects character sequence PI and returns PI token
            c = yytext[0];
            return PI;
          }

%%

