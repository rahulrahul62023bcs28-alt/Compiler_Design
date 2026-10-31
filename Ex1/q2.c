%{
#include<stdio.h>
%}

%%

href=\".*\"	{printf("%s\n", yytext+5);}
.|\n	;

%%

int main(int argc, char *argv[]){
  if (argc >1){
    FILE *fp = fopen(argv[1], "r");
    if (fp) yyin = fp;
  }
  yylex();
  return 0;
}

int yywrap(){
  return 1;
}
