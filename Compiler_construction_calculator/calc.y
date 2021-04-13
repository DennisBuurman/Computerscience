/* C declarations */
%{

#include <stdio.h>
#include <math.h>

int base = 10;

/* Prototypes */
static void yyerror(const char *);

/* import from calc.l */
extern int lineno;
extern int yylex(void);

%}


/* Start symbol */
%start line

/* Tokens */
%token DIGIT
%token PI
%token '|'
%token '(' ')'

/* Tokens with precedence assigned: left (associative), right (associative)*/
%left '-' '+'
%left '%' '/' '*'
%left UMINUS
%left '!'
%right POWER
// highest


/* Rules */
%%

line:
         |
         
         line stmt '\n'
         
         |
         
         line error '\n'
         {
           yyerrok;
         }
         ;


stmt:    expr
         {
           printf("%d\n",$1);
         }
         ;

// Define grammar for all possible expressions
expr:    expr '+' expr
         {
           $$ = $1 + $3;
         }
         
         |
         expr '-' expr
         {
         	 $$ = $1 - $3;
         }
         
         |
         expr '*' expr
         {
           $$ = $1 * $3;
         }
         
         |
         expr '/' expr
         {
           if($3 == 0){
             yyerror("Division by zero error");
             return 0;
           }
           $$ = $1 / $3;
         }
         
         |
         expr '%' expr
         {
           $$ = $1 % $3;
         }
         
         |
         expr '!'
         {
           if($1 > 0){
             // calculates the factorial of expr
             int temp = 1;
             for(int i = $1; i > 0; i--){
                temp *= i;
             }
             $$ = temp;
           } else if ($1 == 0){
             $$ = 1;
           } else{
             yyerror("Negative factorial error");
             return 0;
           }
         }
         
         |
         '-' expr %prec UMINUS
         {
           $$ = -$2;
         }
         
         |
         expr POWER expr
         {
           // calculates expr1**expr2
           int temp = 1;
           int temp1 = $1;
           for(int i = 1; i <= $3; i++){
             temp *= temp1;
           }
           $$ = temp;
         }
         
         |
         '(' expr ')'
         {
           $$ = $2;
         }
         
         |
         '|' expr '|'
         {
           // square root of expr squared is always positive
           $$ = sqrt(($2*$2));
         }
         
         |
         
         number
         ;


number:  DIGIT
         {
           $$ = $1;
         }
         
         |
         
         number DIGIT
         {
           $$ = base * $1 + $2;
         }
         
         |
         
         PI
         {
           // PI gets value 3 assigned 
           $$ = 3;
         }
         ;
%%
/* End of rules, more C code will follow now */


int main(){
  return yyparse();
}

static void yyerror(const char *s){
  fprintf(stderr, "%s\n",s);
}

int yywrap(){
  return(1);
}
