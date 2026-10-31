
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQ 100
#define MAXLOOP 20

int yylex(void);
void yyerror(const char *s);

struct Quadruple
{
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} quad[MAXQ];

int qcount = 0;
int tcount = 0;
int lcount = 0;
int depth = -1;

char start[MAXLOOP][20], end[MAXLOOP][20];
char loopvar[MAXLOOP][20];
char incvar[MAXLOOP][20], incnum[MAXLOOP][20];

void addquad(const char *op, const char *a1,
             const char *a2, const char *res)
{
    if (qcount >= MAXQ)
        return;

    snprintf(quad[qcount].op, 20, "%s", op);
    snprintf(quad[qcount].arg1, 20, "%s", a1);
    snprintf(quad[qcount].arg2, 20, "%s", a2);
    snprintf(quad[qcount].result, 20, "%s", res);
    qcount++;
}

char *newtemp(void)
{
    char *s = malloc(20);
    sprintf(s, "t%d", ++tcount);
    return s;
}

char *newlabel(void)
{
    char *s = malloc(20);
    sprintf(s, "L%d", ++lcount);
    return s;
}

void printquads(void)
{
    int i;

    printf("\n%-12s %-8s %-8s %-8s\n",
           "Op", "Arg1", "Arg2", "Result");
    printf("%-12s %-8s %-8s %-8s\n",
           "----------", "------", "------", "------");

    for (i = 0; i < qcount; i++)
    {
        printf("%-12s %-8s %-8s %-8s\n",
               quad[i].op, quad[i].arg1,
               quad[i].arg2, quad[i].result);
    }
}
%}

%union
{
    char *str;
}

%token FOR
%token <str> ID NUM RELOP
%type <str> expr

%left '+' '-'
%left '*' '/'

%%

program
    : stmts
      {
          printquads();
      }
    ;

stmts
    : stmts stmt
    | /* empty */
    ;

stmt
    : assign
    | for_stmt
    ;

assign
    : ID '=' expr ';'
      {
          addquad("=", $3, "-", $1);
      }
    ;

for_stmt
    : FOR '(' ID '=' NUM ';'
      ID RELOP NUM ';'
      ID '=' ID '+' NUM ')'
      {
          char *a, *b, *t;

          depth++;

          a = newlabel();
          b = newlabel();

          strcpy(start[depth], a);
          strcpy(end[depth], b);
          strcpy(loopvar[depth], $3);
          strcpy(incvar[depth], $13);
          strcpy(incnum[depth], $15);

          addquad("=", $5, "-", $3);
          addquad("label", "-", "-", start[depth]);

          t = newtemp();
          addquad($8, $7, $9, t);
          addquad("if_false", t, "-", end[depth]);

          free(a);
          free(b);
          free(t);
      }
      '{' stmts '}'
      {
          char *t = newtemp();

          addquad("+", incvar[depth], incnum[depth], t);
          addquad("=", t, "-", loopvar[depth]);
          addquad("goto", "-", "-", start[depth]);
          addquad("label", "-", "-", end[depth]);

          free(t);
          depth--;
      }
    ;

expr
    : expr '+' expr
      {
          $$ = newtemp();
          addquad("+", $1, $3, $$);
      }
    | expr '-' expr
      {
          $$ = newtemp();
          addquad("-", $1, $3, $$);
      }
    | expr '*' expr
      {
          $$ = newtemp();
          addquad("*", $1, $3, $$);
      }
    | expr '/' expr
      {
          $$ = newtemp();
          addquad("/", $1, $3, $$);
      }
    | '(' expr ')'
      {
          $$ = $2;
      }
    | ID
      {
          $$ = $1;
      }
    | NUM
      {
          $$ = $1;
      }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Parse error: %s\n", s);
}

int main(void)
{
    printf("Enter the nested for-loop statement:\n");
    yyparse();
    return 0;
}

