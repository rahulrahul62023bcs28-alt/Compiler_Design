%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(const char *s);
%}

%token OPEN CLOSE NEWLINE

%left OPEN CLOSE

%%

lines : lines line
      |
      ;

line  : S NEWLINE 
        { 
            printf("Valid string\n\n");
            return 0;
        }
      | NEWLINE 
      | error NEWLINE 
        { 
            yyerrok; 
        }
      ;

S     : OPEN S CLOSE S
      |
      ;

%%

int main()
{
    printf("Enter strings of parentheses to check (Press Enter to validate, Ctrl+C to exit):\n");
    yyparse();
    return 0;
}

int yyerror(const char *s)
{
    printf("Error: %s\n", s);
    return 1;
}
