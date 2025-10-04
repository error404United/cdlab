#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

int numStates, numSymbols;
int dfa[MAX][MAX];               // dfa[state][symbol_index] = next_state
int finalStates[MAX], numFinal;
int isFinalState[MAX];           // 1 if state is final
int distinguishable[MAX][MAX];   // 1 if (i, j) are distinguishable
int stateGroup[MAX];             // Group number for each state
char symbols[MAX];               // Input symbols

// Check if a state is final
int isFinal(int state) {
    return isFinalState[state];
}

void minimizeDFA() {
    // Step 1: Mark distinguishable final vs non-final states
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < i; j++) {
            if (isFinal(i) != isFinal(j))
                distinguishable[i][j] = 1;
        }
    }

    // Step 2: Iteratively mark indirectly distinguishable states
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < numStates; i++) {
            for (int j = 0; j < i; j++) {
                if (distinguishable[i][j]) continue;

                for (int k = 0; k < numSymbols; k++) {
                    int a = dfa[i][k];
                    int b = dfa[j][k];
                    if (a == b) continue;

                    // Ensure a > b for lookup in lower triangle
                    if (a < b) { int temp = a; a = b; b = temp; }

                    if (distinguishable[a][b]) {
                        distinguishable[i][j] = 1;
                        changed = 1;
                        break;
                    }
                }
            }
        }
    } while (changed);

    // Step 3: Group equivalent states
    int groupNum = 0;
    for (int i = 0; i < numStates; i++) stateGroup[i] = -1;

    for (int i = 0; i < numStates; i++) {
        if (stateGroup[i] == -1) {
            stateGroup[i] = groupNum;
            for (int j = i + 1; j < numStates; j++) {
                int a = i > j ? i : j;
                int b = i > j ? j : i;
                if (!distinguishable[a][b])
                    stateGroup[j] = groupNum;
            }
            groupNum++;
        }
    }

    // Step 4: Print minimized DFA
    printf("\nMinimized DFA Transition Table:\n");
    for (int g = 0; g < groupNum; g++) {
        printf("Group %d: contains states ", g);
        for (int s = 0; s < numStates; s++) {
            if (stateGroup[s] == g)
                printf("q%d ", s);
        }
        printf("\n");
    }

    printf("\nTransitions:\n");
    for (int g = 0; g < groupNum; g++) {
        int rep = -1;
        for (int i = 0; i < numStates; i++) {
            if (stateGroup[i] == g) {
                rep = i; break;
            }
        }

        printf("Group %d: ", g);
        for (int k = 0; k < numSymbols; k++) {
            int next = dfa[rep][k];
            printf("--%c--> Group %d  ", symbols[k], stateGroup[next]);
        }
        printf("\n");
    }

    printf("\nFinal states of minimized DFA: ");
    for (int g = 0; g < groupNum; g++) {
        for (int i = 0; i < numStates; i++) {
            if (stateGroup[i] == g && isFinal(i)) {
                printf("Group %d ", g);
                break;
            }
        }
    }
    printf("\n");
}

int main() {
    printf("Enter number of DFA states: ");
    scanf("%d", &numStates);

    printf("Enter number of input symbols: ");
    scanf("%d", &numSymbols);

    printf("Enter the input symbols (e.g. a b): ");
    for (int i = 0; i < numSymbols; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter DFA transitions (state symbol_index to_state):\n");
    printf("Use symbol index: 0 for '%c', 1 for '%c', etc.\n", symbols[0], symbols[1]);
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numSymbols; j++) {
            scanf("%d", &dfa[i][j]);
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &numFinal);

    printf("Enter final states: ");
    for (int i = 0; i < numFinal; i++) {
        scanf("%d", &finalStates[i]);
        isFinalState[finalStates[i]] = 1;
    }

    minimizeDFA();
    return 0;
}

