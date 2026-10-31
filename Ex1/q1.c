%{
#include<stdio.h>
int w = 0, c = 0, l = 0;
%}

%%

\n	{l++;}
[a-zA-Z0-9]+	{w++; c+=yyleng;}
" "	{;}
.	{c++;}

%%

int main(int argc, char *argv[]){
  if (argc > 1){
    FILE *fp = fopen(argv[1], "r");
    if(fp) yyin = fp;
  }
  yylex();
  printf("Words = %d\n", w);
  printf("Characters = %d\n", c);
  printf("Lines = %d\n", l);
  return 0;
}

int yywrap(){
  return 1;
}
