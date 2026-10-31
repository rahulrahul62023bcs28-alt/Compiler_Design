%{
#include <stdio.h>

int f = 0;
%}

%%

"int"|"float"|"char"|"void" {
    if(f == 0)
    {
        printf("RETURN TYPE : %s\n", yytext);
        f = 1;
    }
    else if(f == 2)
    {
        printf("PARAMETER : %s", yytext);
    }
    else
    {
        printf("IDENTIFIER : %s\n", yytext);
    }
}

[a-zA-Z][a-zA-Z0-9]* {
    if(f == 1)
    {
        printf("FUNCTION NAME : %s\n", yytext);
        f = 2;
    }
    else if(f == 2)
    {
        printf(" %s\n", yytext);
    }
    else
    {
        printf("IDENTIFIER : %s\n", yytext);
    }
}

["return"] {
    printf("KEYWORD : %s\n", yytext);
}

"(" {
    printf("PUNCTUATION : %s\n", yytext);
}

"," {
    printf("PUNCTUATION : %s\n", yytext);
}

")" {
    printf("PUNCTUATION : %s\n", yytext);
    f = 3;
}

"{"|"}"|";" {
    printf("PUNCTUATION : %s\n", yytext);
}

[+\-*/=] {
    printf("OPERATOR : %s\n", yytext);
}

[ \t\n]+ ;

. {
    /* Ignore other characters */
}

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
