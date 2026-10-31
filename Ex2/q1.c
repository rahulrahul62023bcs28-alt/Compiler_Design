%{
#include <stdio.h>
%}

%%

[a-zA-Z_][a-zA-Z0-9]*    { printf("IDENTIFIER 	 : %s\n", yytext); }
[0-9]+\.[0-9]+           { printf("FLOAT CONSTANT   : %s\n", yytext); }
[0-9]+                   { printf("INTEGER CONSTANT : %s\n", yytext); }
[=\+\-\*/]               { printf("OPERATOR         : %s\n", yytext); }
[;]                      { printf("PUNCTUATION      : %s\n", yytext); }
[ \t\n]                  { }

%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]){
  if (argc>1){
   FILE *fp = fopen(argv[1], "r");
   if (fp) yyin = fp;
  }
  yylex();
  return 0;
}
