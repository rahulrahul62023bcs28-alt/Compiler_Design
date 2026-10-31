%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token OPEN CLOSE COMMA A NEWLINE

%%

lines : lines line
      |
      ;

line  : S NEWLINE 
        { 
            printf("Valid string\n\n"); 
        }
      | NEWLINE
      | error NEWLINE
        {
            yyerrok;
        }
      ;

S : OPEN L CLOSE
  | A
  ;

L : L COMMA S
  | S
  ;

%%

int main()
{
    printf("Enter strings of parentheses to check (Press Enter to validate, Ctrl+C to exit):\n");
    yyparse();
    return 0;
}

void yyerror(const char *s)
{
    printf("Error: %s\n\n", s);
}
