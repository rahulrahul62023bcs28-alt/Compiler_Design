%{
#include "y.tab.h"
%}

%%

"("     { return OPEN; }
")"     { return CLOSE; }
"\n"    { return NEWLINE; }
.       { return yytext[0]; }

%%

int yywrap(){return 1;}