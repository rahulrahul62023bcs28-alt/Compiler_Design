%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

struct Quad
{
    char *op, *arg1, *arg2, *result;
};

struct Quad q[100];
int n = 0, temp = 0, label = 0;

char *loop_start, *loop_end;
char *update_lhs, *update_rhs, *update_val;

char *newtemp()
{
    char *s = malloc(20);
    sprintf(s, "t%d", ++temp);
    return s;
}

char *newlabel()
{
    char *s = malloc(20);
    sprintf(s, "L%d", ++label);
    return s;
}

void addquad(char *op, char *a1, char *a2, char *r)
{
    q[n].op = strdup(op);
    q[n].arg1 = a1 ? strdup(a1) : strdup("-");
    q[n].arg2 = a2 ? strdup(a2) : strdup("-");
    q[n].result = strdup(r);
    n++;
}

void emit_label(char *l)
{
    addquad("label", "-", "-", l);
}

void printquads()
{
    int i;

    printf("\nTHREE ADDRESS CODE - QUADRUPLES TABLE\n");
    printf("---------------------------------------------------------------\n");
    printf("%-5s %-12s %-12s %-12s %-12s\n",
           "No.", "Operator", "Arg1", "Arg2", "Result");
    printf("---------------------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("%-5d %-12s %-12s %-12s %-12s\n",
               i, q[i].op, q[i].arg1,
               q[i].arg2, q[i].result);
    }

    printf("---------------------------------------------------------------\n");
}
%}

%union {
    char *str;
}

%token <str> ID NUM RELOP
%token FOR

%type <str> exp

%left '+' '-'
%left '*' '/'

%%

program:
    init_stmt ';'
    FOR
    {
        loop_start = newlabel();
        loop_end = newlabel();
        emit_label(loop_start);
    }
    '(' loop_init ';' condition ';' update ')'
    '{' body_stmt ';' '}'
    {
        char *t = newtemp();

        addquad("+", update_rhs, update_val, t);
        addquad("=", t, "-", update_lhs);
        addquad("goto", "-", "-", loop_start);
        emit_label(loop_end);
    }
    final_stmt ';'
    {
        printquads();
    }
    ;

init_stmt:
    ID '=' exp
    {
        addquad("=", $3, "-", $1);
    }
    ;

loop_init:
    ID '=' exp
    {
        addquad("=", $3, "-", $1);
    }
    ;

condition:
    ID RELOP NUM
    {
        char *t = newtemp();

        addquad($2, $1, $3, t);
        addquad("if_false", t, "-", loop_end);
    }
    ;

update:
    ID '=' ID '+' NUM
    {
        update_lhs = strdup($1);
        update_rhs = strdup($3);
        update_val = strdup($5);
    }
    ;

body_stmt:
    ID '=' exp
    {
        addquad("=", $3, "-", $1);
    }
    ;

final_stmt:
    ID '=' exp
    {
        addquad("=", $3, "-", $1);
    }
    ;

exp:
    exp '+' exp
    {
        $$ = newtemp();
        addquad("+", $1, $3, $$);
    }
    | exp '-' exp
    {
        $$ = newtemp();
        addquad("-", $1, $3, $$);
    }
    | exp '*' exp
    {
        $$ = newtemp();
        addquad("*", $1, $3, $$);
    }
    | exp '/' exp
    {
        $$ = newtemp();
        addquad("/", $1, $3, $$);
    }
    | ID
    {
        $$ = strdup($1);
    }
    | NUM
    {
        $$ = strdup($1);
    }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(void)
{
    printf("Enter code snippet:\n");
    yyparse();
    return 0;
}
