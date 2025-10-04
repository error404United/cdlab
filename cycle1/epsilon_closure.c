// cycle1_q1_epsilon_closure.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int nStates;
int epsilonTransitions[MAX][MAX];
int epsilonCount[MAX];
int visited[MAX];
int epsilonClosure[MAX][MAX];
int closureCount[MAX];

void dfs(int state, int current) {
    for (int i = 0; i < epsilonCount[state]; i++) {
        int next = epsilonTransitions[state][i];
        if (!visited[next]) {
            visited[next] = 1;
            epsilonClosure[current][closureCount[current]++] = next;
            dfs(next, current);
        }
    }
}

void computeClosures() {
    for (int i = 0; i < nStates; i++) {
        memset(visited, 0, sizeof(visited));
        visited[i] = 1;
        closureCount[i] = 0;
        epsilonClosure[i][closureCount[i]++] = i;
        dfs(i, i);
    }
}

void printClosures() {
    printf("\nEpsilon closures of all states:\n");
    for (int i = 0; i < nStates; i++) {
        printf("ε-closure(q%d): { ", i);
        for (int j = 0; j < closureCount[i]; j++) {
            printf("q%d ", epsilonClosure[i][j]);
        }
        printf("}\n");
    }
}

int main() {
    int nSymbols, nTransitions;
    char symbols[20];
    int from, to;
    char symbol;

    printf("Enter number of states: ");
    scanf("%d", &nStates);

    printf("Enter number of input symbols (excluding epsilon): ");
    scanf("%d", &nSymbols);
    printf("Enter the symbols:\n");
    for (int i = 0; i < nSymbols; i++) {
        scanf(" %c", &symbols[i]);
    }

    printf("Enter number of transitions: ");
    scanf("%d", &nTransitions);

    for (int i = 0; i < nStates; i++)
        epsilonCount[i] = 0;

    printf("Enter transitions (from symbol to):\n");
    for (int i = 0; i < nTransitions; i++) {
        scanf("%d %c %d", &from, &symbol, &to);
        if (symbol == 'e') {
            epsilonTransitions[from][epsilonCount[from]++] = to;
        }
    }

    computeClosures();
    printClosures();

    return 0;
}

