#include <stdio.h>
#include <string.h>

#define MAX 100
#define MAX_STATE 10

char alphabets[MAX];
char inputString[MAX];

int transitionTable[MAX_STATE][MAX];
int is_final[MAX_STATE];

int startingState;

int getSymbolIndex(char symbol, int numSymbols)
{
    for (int i = 0; i < numSymbols; i++)
    {
        if (alphabets[i] == symbol)
            return i;
    }

    return -1;
}

int main()
{
    int numStates, numSymbols;
    int numFinalStates, finalState;
    char choice;

    printf("Enter the number of states: ");
    scanf("%d", &numStates);

    printf("Enter the starting state: ");
    scanf("%d", &startingState);

    for (int i = 0; i < numStates; i++)
        is_final[i] = 0;

    printf("Enter the number of accepting states: ");
    scanf("%d", &numFinalStates);

    printf("Enter the accepting state(s): ");
    for (int i = 0; i < numFinalStates; i++)
    {
        scanf("%d", &finalState);
        is_final[finalState] = 1;
    }

    printf("Enter the number of alphabets: ");
    scanf("%d", &numSymbols);

    printf("Enter the alphabets: ");
    for (int i = 0; i < numSymbols; i++)
        scanf(" %c", &alphabets[i]);

    printf("Enter the transition table:\n");

    for (int i = 0; i < numStates; i++)
    {
        for (int j = 0; j < numSymbols; j++)
        {
            scanf("%d", &transitionTable[i][j]);
        }
    }

    /* Print transition table */
    printf("\n========== TRANSITION TABLE ==========\n");

    printf("State\t");
    for (int j = 0; j < numSymbols; j++)
        printf("%c\t", alphabets[j]);

    printf("\n");

    for (int i = 0; i < numStates; i++)
    {
        printf("Q%d\t", i);

        for (int j = 0; j < numSymbols; j++)
            printf("Q%d\t", transitionTable[i][j]);

        if (i == startingState)
            printf("<-- Starting State");

        if (is_final[i])
            printf(" <-- Accepting State");

        printf("\n");
    }

    printf("=======================================\n");

    /* Repeatedly accept input strings */
    do
    {
        printf("\nEnter the input string: ");
        scanf("%s", inputString);

        int currentState = startingState;

        printf("\n========== DFA TRANSITIONS ==========\n");
        printf("Starting State: Q%d\n\n", currentState);

        int invalid = 0;

        for (int i = 0; i < strlen(inputString); i++)
        {
            char symbol = inputString[i];

            int symbolIndex = getSymbolIndex(symbol, numSymbols);

            if (symbolIndex == -1)
            {
                printf("Q%d -- %c --> Invalid\n",
                       currentState, symbol);

                printf("\nReason: '%c' is not present in the alphabet.\n",
                       symbol);

                invalid = 1;
                break;
            }

            int nextState = transitionTable[currentState][symbolIndex];

            printf("Q%d -- %c --> Q%d\n",
                   currentState, symbol, nextState);

            currentState = nextState;
        }

        printf("=====================================\n");

        if (!invalid)
        {
            printf("\nFinal State: Q%d\n", currentState);

            if (is_final[currentState])
            {
                printf("Reason: Q%d is an accepting state.\n",
                       currentState);

                printf("%s - Valid\n", inputString);
            }
            else
            {
                printf("Reason: Q%d is not an accepting state.\n",
                       currentState);

                printf("%s - Invalid\n", inputString);
            }
        }
        else
        {
            printf("%s - Invalid\n", inputString);
        }

        printf("\nDo you want to enter another string? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    printf("\nProgram ended.\n");

    return 0;
}