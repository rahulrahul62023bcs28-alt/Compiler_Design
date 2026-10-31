
%{
#include "parse.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

"for"                        { return FOR; }
"=="|"!="|"<="|">="|"<"|">" { yylval.str = strdup(yytext); return RELOP; }
[0-9]+                       { yylval.str = strdup(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*       { yylval.str = strdup(yytext); return ID; }

[ \t\r\n]+                   ;
.                            { return yytext[0]; }

%%

int yywrap(void)
{
    return 1;
}
