%{
#include<stdio.h>
%}

%%

"<="	{printf("Less than or equal\n");}
">="	{printf("Greater than or equal\n");}
"=="	{printf("Equal to\n");}
"!="	{printf("Not equal to\n");}
"<"	{printf("Less than\n");}
">"	{printf("Greater than\n");}
.|\n	{;}

%%

int main(int argc, char *argv[]){
  if(argc>1){
    FILE *fp = fopen(argv[1],"r");
    if (fp) yyin = fp;
  }
  yylex();
  return 0;
}

int yywrap(){
  return 1;
}
