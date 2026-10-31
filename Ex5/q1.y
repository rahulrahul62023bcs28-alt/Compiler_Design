%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUM NEWLINE PLUS MINUS MULT DIV MOD POW OPEN CLOSE

%%

lines : lines line
      |
      ;

line  : E NEWLINE 
        { 
            printf("Result: %d\n\n", $1); 
        }
      | NEWLINE
      | error NEWLINE
        {
            yyerrok;
        }
      ;

E     : E PLUS T      { $$ = $1 + $3; }
      | E MINUS T     { $$ = $1 - $3; }
      | T             { $$ = $1; }
      ;

T     : T MULT F      { $$ = $1 * $3; }
      | T DIV F       { 
                        if ($3 == 0) {
                            yyerror("Division by zero condition occurred");
                            YYERROR; 
                        } else {
                            $$ = $1 / $3; 
                        }
                    }
      | T MOD F       { 
                        if ($3 == 0) {
                            yyerror("Modulo by zero condition occurred");
                            YYERROR; 
                        } else {
                            $$ = $1 % $3; 
                        }
                    }
      | F             { $$ = $1; }
      ;

F     : P POW F       { $$ = (int)pow($1, $3); }
      | P             { $$ = $1; }
      ;

P     : MINUS P       { $$ = -$2; }
      | OPEN E CLOSE  { $$ = $2; }
      | NUM           { $$ = $1; }
      ;

%%

int main()
{
    printf("Enter math expressions (Press Enter to evaluate, Ctrl+C to exit):\n");
    yyparse();
    return 0;
}

void yyerror(const char *s)
{
    printf("Error: %s\n\n", s);
}