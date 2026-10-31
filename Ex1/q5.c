%{
#include<stdio.h>
int v = 0, c = 0;
%}

%%

[AEIOUaeiou]	{v++;}

[B-DF-HJ-NP-TV-Zb-df-hj-np-tv-z]	{c++;}

%%

int main(int argc, char *argv[]){
  if (argc>1){
   FILE *fp = fopen(argv[1], "r");
   if (fp) yyin = fp;
  }
  yylex();
  printf("Vowels = %d\n", v);
  printf("Consonants = %d\n", c);
  return 0;
}

int yywrap(){
  return 1;
}
