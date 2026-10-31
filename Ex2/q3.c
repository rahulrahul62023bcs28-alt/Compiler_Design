%{
#include <stdio.h>

int f = 0;
%}

%%

"struct" {
    printf("KEYWORD : %s\n", yytext);
    f = 1;
}

"int"|"char"|"float"|"double" {
    printf("DATA TYPE : %s\n", yytext);
}

[a-zA-Z][a-zA-Z0-9]* {
    if(f == 1)
    {
        printf("STRUCT TAG : %s\n", yytext);
        f = 2;
    }
    else
    {
        printf("IDENTIFIER : %s\n", yytext);
    }
}

"{" {
    printf("PUNCTUATION : %s\n", yytext);
}

"}" {
    printf("PUNCTUATION : %s\n", yytext);
    f = 0;
}

";"|"," {
    printf("PUNCTUATION : %s\n", yytext);
}

[ \t\n]+ ;

. ;

%%

int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        FILE *fp = fopen(argv[1], "r");

        if(fp)
        {
            yyin = fp;
        }
    }

    yylex();

    return 0;
}
