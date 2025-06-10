// Equipe: DANIEL SANTANA LIMA e MAURICIO SANTANA DA SILVA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define MAX_WORD 100

char stack[MAX][MAX_WORD];
int top = -1;

void push(const char *word) {
    strcpy(stack[++top], word);
}

void pop() {
    if (top >= 0) top--;
}

char* peek() {
    return top >= 0 ? stack[top] : "";
}

int main() {
    char word[MAX_WORD];
    char line[4096];

    while (fgets(line, sizeof(line), stdin)) {
        int i = 0;
        top = -1;
        char *token = strtok(line, " \n");
        char *tempStack[MAX];
        int tempTop = -1;
        int popCount = 0;

        while (token) {
            // Remover enquanto o topo for menor que o novo elemento (ordem alfabética)
            while (top >= 0 && strcmp(token, peek()) < 0) {
                tempStack[++tempTop] = strdup(peek());
                pop();
                popCount++;
            }

            if (popCount == 1)
                printf("push−%s 1x−pop ", token);
            else if (popCount > 1)
                printf("push−%s %dx−pop ", token, popCount);
            else
                printf("push−%s ", token);

            push(token);

            // Reempilhar os elementos removidos
            while (tempTop >= 0) {
                push(tempStack[tempTop]);
                printf("push−%s ", tempStack[tempTop]);
                free(tempStack[tempTop--]);
            }

            popCount = 0;
            token = strtok(NULL, " \n");
        }

        printf("\n");
    }

    return 0;
}
