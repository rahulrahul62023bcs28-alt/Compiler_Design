%{
#include "y.tab.h"
%}

%%

"("     { return OPEN; }
")"     { return CLOSE; }
","     { return COMMA; }
"a"     { return A; }
"\n"    { return NEWLINE; }

[ \t]+  ; /* Clear out spaces or tabs instantly */

.       { return yytext[0]; }

%%

int yywrap(){return 1;}
