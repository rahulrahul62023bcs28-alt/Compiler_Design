%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror(char *s);
%}

%%
line : S '\n'   { printf("Valid string\n"); exit(0); }
     ;

S : 'a' S 'a'
  | 'b' S 'b'
  | 'a' 'a'
  | 'b' 'b'
  ;
%%

int main() {
    printf("Enter string: ");
    yyparse();
    return 0;
}

int yyerror(char *s) {
    printf("Invalid string\n");
    exit(0);
}
