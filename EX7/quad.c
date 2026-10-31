#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAX_QUADS 100

struct quadruple
{
    char result[32];
    char operator[20];
    char operand1[32];
    char operand2[32];
};

int isTemp(const char *name)
{
    int i;

    if(name[0] != 't' || name[1] == '\0')
        return 0;

    for(i = 1; name[i] != '\0'; i++)
        if(name[i] < '0' || name[i] > '9')
            return 0;

    return 1;
}

int isControl(const char *op)
{
    return strcmp(op, "ifFalse") == 0 ||
           strcmp(op, "ifTrue") == 0 ||
           strcmp(op, "goto") == 0 ||
           strcmp(op, "label") == 0;
}

int isRelational(const char *op)
{
    return strcmp(op, "<") == 0 ||
           strcmp(op, ">") == 0 ||
           strcmp(op, "<=") == 0 ||
           strcmp(op, ">=") == 0 ||
           strcmp(op, "==") == 0 ||
           strcmp(op, "!=") == 0;
}

void display(struct quadruple *q, int n)
{
    int i;

    printf("RI\tOP\t\tO1\tO2\tRES\n");
    for(i = 0; i < n; i++)
        printf("%d\t%-12s\t%s\t%s\t%s\n", i, q[i].operator,
               q[i].operand1, q[i].operand2, q[i].result);
}

int readNumber(const char *text, int *value)
{
    char *end;
    long number;

    errno = 0;
    number = strtol(text, &end, 10);

    if(errno || end == text || *end != '\0' ||
       number < INT_MIN || number > INT_MAX)
        return 0;

    *value = (int)number;
    return 1;
}

void replaceUses(struct quadruple *q, int start, int n,
                 const char *oldName, const char *newName)
{
    int j;

    for(j = start; j < n; j++)
    {
        if(strcmp(q[j].operand1, oldName) == 0)
            strcpy(q[j].operand1, newName);

        if(strcmp(q[j].operand2, oldName) == 0)
            strcpy(q[j].operand2, newName);
    }
}

void removeQuad(struct quadruple *q, int *n, int index)
{
    int k;

    for(k = index; k < *n - 1; k++)
        q[k] = q[k + 1];

    (*n)--;
}

/* Do not optimize across labels or jumps; they mark control-flow boundaries. */
void eliminate(struct quadruple *q, int *n)
{
    int i, j, k, safe;
    char oldName[32], newName[32];

    for(i = 0; i < *n - 1; i++)
    {
        if(isControl(q[i].operator) ||
           strcmp(q[i].operator, "=") == 0 ||
           q[i].operand2[0] == '\0' ||
           strcmp(q[i].operand2, "---") == 0)
            continue;

        if(strcmp(q[i].result, q[i].operand1) == 0 ||
           strcmp(q[i].result, q[i].operand2) == 0)
            continue;

        for(j = i + 1; j < *n; )
        {
            if(isControl(q[j].operator))
                break;

            if(strcmp(q[i].operator, q[j].operator) == 0 &&
               strcmp(q[i].operand1, q[j].operand1) == 0 &&
               strcmp(q[i].operand2, q[j].operand2) == 0)
            {
                safe = 1;

                for(k = i + 1; k < j; k++)
                {
                    if(isControl(q[k].operator) ||
                       strcmp(q[k].result, q[i].operand1) == 0 ||
                       strcmp(q[k].result, q[i].operand2) == 0 ||
                       strcmp(q[k].result, q[i].result) == 0)
                    {
                        safe = 0;
                        break;
                    }
                }

                if(safe)
                {
                    if(isTemp(q[j].result))
                    {
                        strcpy(oldName, q[j].result);
                        strcpy(newName, q[i].result);
                        replaceUses(q, j + 1, *n, oldName, newName);
                        removeQuad(q, n, j);
                        continue;
                    }

                    strcpy(q[j].operator, "=");
                    strcpy(q[j].operand1, q[i].result);
                    strcpy(q[j].operand2, "---");
                }
            }

            j++;
        }
    }
}

void applyConstantFolding(struct quadruple *q, int *n)
{
    int i, a, b;
    long long value;

    for(i = 0; i < *n; i++)
    {
        if(isControl(q[i].operator) ||
           isRelational(q[i].operator) ||
           strcmp(q[i].operator, "=") == 0)
            continue;

        if(readNumber(q[i].operand1, &a) &&
           readNumber(q[i].operand2, &b))
        {
            switch(q[i].operator[0])
            {
                case '+': value = (long long)a + b; break;
                case '-': value = (long long)a - b; break;
                case '*': value = (long long)a * b; break;
                case '/':
                    if(b == 0 || (a == INT_MIN && b == -1))
                        continue;
                    value = a / b;
                    break;
                default: continue;
            }

            if(value < INT_MIN || value > INT_MAX)
                continue;

            snprintf(q[i].operand1, sizeof(q[i].operand1), "%lld", value);
            strcpy(q[i].operand2, "---");
            strcpy(q[i].operator, "=");
        }
    }
}

void applyCopyPropagation(struct quadruple *q, int *n)
{
    int i, j, dummy, used;
    char source[32], target[32];

    for(i = 0; i < *n; i++)
    {
        if(isControl(q[i].operator) ||
           strcmp(q[i].operator, "=") != 0 ||
           strcmp(q[i].operand2, "---") != 0)
            continue;

        strcpy(source, q[i].operand1);
        strcpy(target, q[i].result);

        if(!readNumber(source, &dummy) && !isTemp(source))
            continue;

        used = 0;

        for(j = i + 1; j < *n; j++)
        {
            if(isControl(q[j].operator))
                break;

            if(strcmp(q[j].operand1, target) == 0)
            {
                strcpy(q[j].operand1, source);
                used = 1;
            }

            if(strcmp(q[j].operand2, target) == 0)
            {
                strcpy(q[j].operand2, source);
                used = 1;
            }

            if(strcmp(q[j].result, target) == 0 ||
               strcmp(q[j].result, source) == 0)
                break;
        }

        if(used)
        {
            removeQuad(q, n, i);
            i--;
        }
    }
}

int isDeadBranch(struct quadruple *q)
{
    int value;

    if(!readNumber(q->operand1, &value))
        return 0;

    if(strcmp(q->operator, "ifFalse") == 0 && value != 0)
        return 1;

    if(strcmp(q->operator, "ifTrue") == 0 && value == 0)
        return 1;

    return 0;
}

void applyDeadCodeElimination(struct quadruple *q, int *n)
{
    int i, j, used;

    for(i = *n - 1; i >= 0; i--)
    {
        if(isDeadBranch(&q[i]))
            removeQuad(q, n, i);
    }

    for(i = *n - 1; i >= 0; i--)
    {
        if(!isTemp(q[i].result) || isControl(q[i].operator))
            continue;

        used = 0;

        for(j = i + 1; j < *n; j++)
        {
            if(isControl(q[j].operator))
            {
                used = 1;
                break;
            }

            if(strcmp(q[i].result, q[j].operand1) == 0 ||
               strcmp(q[i].result, q[j].operand2) == 0)
            {
                used = 1;
                break;
            }

            if(strcmp(q[i].result, q[j].result) == 0)
                break;
        }

        if(!used)
            removeQuad(q, n, i);
    }
}

int validOperator(const char *op)
{
    return strcmp(op, "=") == 0 ||
           strcmp(op, "+") == 0 ||
           strcmp(op, "-") == 0 ||
           strcmp(op, "*") == 0 ||
           strcmp(op, "/") == 0 ||
           isRelational(op) ||
           isControl(op);
}

int main(void)
{
    struct quadruple quad[MAX_QUADS];
    int n, i;

    printf("Enter the number of quadruples: ");
    if(scanf("%d", &n) != 1 || n < 1 || n > MAX_QUADS)
    {
        fprintf(stderr, "Enter a number from 1 to %d.\n", MAX_QUADS);
        return 1;
    }

    printf("Format: Operator Operand-1 Operand-2 Result\n");
    printf("Use --- for an unused operand. Temporaries: t1, t2, ...\n");
    printf("Control operators: ifFalse, ifTrue, goto, label\n");

    for(i = 0; i < n; i++)
    {
        printf("Enter the quadruple [%d]: ", i);

        if(scanf("%19s %31s %31s %31s", quad[i].operator,
                 quad[i].operand1, quad[i].operand2,
                 quad[i].result) != 4)
        {
            fprintf(stderr, "Invalid quadruple.\n");
            return 1;
        }

        if(!validOperator(quad[i].operator))
        {
            fprintf(stderr, "Unsupported operator: %s\n", quad[i].operator);
            return 1;
        }

        if(strcmp(quad[i].operator, "=") == 0 ||
           strcmp(quad[i].operator, "ifFalse") == 0 ||
           strcmp(quad[i].operator, "ifTrue") == 0 ||
           strcmp(quad[i].operator, "goto") == 0 ||
           strcmp(quad[i].operator, "label") == 0)
            strcpy(quad[i].operand2, "---");
    }

    printf("\nTAC in Quadruples\n");
    display(quad, n);

    eliminate(quad, &n);
    printf("\nAfter Common Subexpression Elimination\n");
    display(quad, n);

    applyCopyPropagation(quad, &n);
    printf("\nAfter Copy Propagation\n");
    display(quad, n);

    applyConstantFolding(quad, &n);
    printf("\nAfter Constant Folding\n");
    display(quad, n);
    applyDeadCodeElimination(quad, &n);
    printf("\nAfter Dead Code Elimination\n");
    display(quad, n);

    return 0;
}
