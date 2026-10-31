%{
#include <stdio.h>
%}

%x UNARY

id      [a-zA-Z_][a-zA-Z0-9_]*
digit   [0-9]+

%%

<INITIAL>{id} {
    printf("IDENTIFIER : %s\n", yytext);
}

<INITIAL>{digit} {
    printf("NUMBER : %s\n", yytext);
}

<INITIAL>"=" {
    printf("OPERATOR : %s\n", yytext);
    BEGIN(UNARY);
}

<INITIAL>[+*/%] {
    printf("OPERATOR : %s\n", yytext);
    BEGIN(UNARY);
}

<INITIAL>"-" {
    printf("SUBTRACTION : %s\n", yytext);
    BEGIN(UNARY);
}

<INITIAL>"(" {
    printf("LEFT PARENTHESIS\n");
    BEGIN(UNARY);
}

<INITIAL>")" {
    printf("RIGHT PARENTHESIS\n");
}

<INITIAL>[ \t\n]+ ;

<UNARY>"-" {
    printf("UNARY MINUS : %s\n", yytext);
}

<UNARY>{id} {
    printf("IDENTIFIER : %s\n", yytext);
    BEGIN(INITIAL);
}

<UNARY>{digit} {
    printf("NUMBER : %s\n", yytext);
    BEGIN(INITIAL);
}

<UNARY>"(" {
    printf("LEFT PARENTHESIS\n");
    BEGIN(UNARY);
}

<UNARY>[ \t\n]+ ;



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
            yyin = fp;
    }

    yylex();

    return 0;
}
