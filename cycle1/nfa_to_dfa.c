#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 10
#define MAX_DFA_STATES 100

int nfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int nfaCount[MAX_STATES][MAX_SYMBOLS];
int nfaStates, symbolsCount;
char symbols[MAX_SYMBOLS];

// DFA state representation
int dfaStates[MAX_DFA_STATES][MAX_STATES]; // Each DFA state is a set of NFA states
int dfaStateCount = 0;
int dfaTrans[MAX_DFA_STATES][MAX_SYMBOLS];

int isSameState(int a[], int b[]) {
    for (int i = 0; i < nfaStates; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int stateExists(int stateSet[][MAX_STATES], int count, int state[]) {
    for (int i = 0; i < count; i++) {
        if (isSameState(stateSet[i], state)) return i;
    }
    return -1;
}

void addToSet(int set[], int val) {
    if (!set[val])
        set[val] = 1;
}

void printDFA() {
    printf("\nDFA Transition Table:\n");
    for (int i = 0; i < dfaStateCount; i++) {
        printf("DFA state %d:", i);
        for (int k = 0; k < symbolsCount; k++) {
            printf(" --%c--> %d", symbols[k], dfaTrans[i][k]);
        }
        printf("\n");
    }
}


int main() {
    printf("Enter number of NFA states: ");
    scanf("%d", &nfaStates);

    printf("Enter number of input symbols: ");
    scanf("%d", &symbolsCount);

    printf("Enter the input symbols: ");
    for (int i = 0; i < symbolsCount; i++) {
        scanf(" %c", &symbols[i]);
    }

    int nTransitions;
    printf("Enter number of NFA transitions: ");
    scanf("%d", &nTransitions);
    printf("Enter transitions (from_state symbol to_state):\n");

    for (int i = 0; i < nTransitions; i++) {
        int from, to;
        char sym;
        scanf("%d %c %d", &from, &sym, &to);

        int symIdx = -1;
        for (int j = 0; j < symbolsCount; j++) {
            if (symbols[j] == sym) {
                symIdx = j;
                break;
            }
        }

        if (symIdx != -1) {
            nfa[from][symIdx][nfaCount[from][symIdx]++] = to;
        } else {
            printf("Invalid symbol.\n");
            return 1;
        }
    }

    // Initial DFA state is NFA state 0
    int initial[MAX_STATES] = {0};
    initial[0] = 1;
    memcpy(dfaStates[0], initial, sizeof(initial));
    dfaStateCount = 1;

    for (int i = 0; i < dfaStateCount; i++) {
        for (int k = 0; k < symbolsCount; k++) {
            int newSet[MAX_STATES] = {0};

            // For each NFA state in the current DFA state
            for (int s = 0; s < nfaStates; s++) {
                if (dfaStates[i][s]) {
                    for (int t = 0; t < nfaCount[s][k]; t++) {
                        addToSet(newSet, nfa[s][k][t]);
                    }
                }
            }

            int existingIndex = stateExists(dfaStates, dfaStateCount, newSet);
            if (existingIndex == -1) {
                memcpy(dfaStates[dfaStateCount], newSet, sizeof(newSet));
                dfaTrans[i][k] = dfaStateCount;
                dfaStateCount++;
            } else {
                dfaTrans[i][k] = existingIndex;
            }
        }
    }

    printDFA();
    return 0;
}

