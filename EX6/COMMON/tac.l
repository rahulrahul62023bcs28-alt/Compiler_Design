%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

%%

"if"                        { return IF; }
[0-9]+                      { yylval.str = strdup(yytext); return NUM; }
[a-zA-Z_][a-zA-Z_0-9]*     { yylval.str = strdup(yytext); return ID; }
"<="|">="|"=="|"!="|"<"|">" { yylval.str = strdup(yytext); return RELOP; }
[ \t\n]+                    { /* skip whitespace */ }
.                           { return yytext[0]; }

%%

int yywrap(void) { return 1; }

