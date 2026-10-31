%{
#include "y.tab.h"
%}

%%

"if"        { return IF; }
"else"      { return ELSE; }
"id"        { return ID; }
"num"       { return NUM; }
"<"|"&lt;"  { return LT; }
"="         { return ASSIGN; }
"-"         { return MINUS; }
"("         { return OPEN; }
")"         { return CLOSE; }
"{"         { return OPENB; }
"}"         { return CLOSEB; }
";"         { return SEMICOLON; }
"\n"        { return NEWLINE;}
[ \t]+    ;
.       { return yytext[0]; }

%%

int yywrap(){return 1;}