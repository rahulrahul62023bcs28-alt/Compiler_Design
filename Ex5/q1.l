%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUM; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"^"         { return POW; }
"("         { return OPEN; }
")"         { return CLOSE; }
"\n"        { return NEWLINE; }
[ \t]+      ;
.           { return yytext[0]; }

%%

int yywrap() { return 1; }
