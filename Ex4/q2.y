%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token IF ELSE ID NUM LT ASSIGN MINUS
%token OPEN CLOSE OPENB CLOSEB SEMICOLON NEWLINE

%%

lines : lines line
      |
      ;

line  : S NEWLINE 
        { 
            printf("Valid\n\n"); 
        }
      | NEWLINE
      | error NEWLINE
        {
            yyerrok;
        }
      ;

S : IF OPEN C CLOSE OPENB A CLOSEB ELSE OPENB A CLOSEB
  ;

C : ID LT NUM
  ;

A : ID ASSIGN E SEMICOLON
  ;

E : ID
  | NUM
  | ID MINUS NUM
  ;

%%

int main()
{
    printf("Enter string (Press Enter to validate, Ctrl+C to exit):\n");
    yyparse();
    return 0;
}

void yyerror(const char *s)
{
    printf("Error: %s\n\n", s);
}
