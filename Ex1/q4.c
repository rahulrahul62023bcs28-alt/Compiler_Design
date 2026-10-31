%{
#include<stdio.h>
%}

%%

[a-z] {if (yytext[0] <= 'w') 
         printf("%c", yytext[0]+3);
       else printf("%c", yytext[0]-23);}

[A-Z] {if (yytext[0] <= 'W') 
         printf("%c", yytext[0]+3);
       else printf("%c", yytext[0]-23);}

.|\n	{;}

%%

int main(int argc, char *argv[]){
  if(argc>1){
    FILE *fp =  fopen(argv[1], "r");
    if (fp) yyin = fp;
  }
  yylex();
  printf("\n");
  return 0;
}

int yywrap(){
  return 1;
}
