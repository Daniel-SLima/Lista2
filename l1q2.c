#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

char **stack = NULL;
int capacity = 4096; 
int top = -1;

void push(const char *word) {
    if (top + 1 >= capacity) {
        capacity *= 2;
        char **new_stack = realloc(stack, capacity * sizeof(char*));
        if (!new_stack) {
            fprintf(stderr, "Erro de memória no realloc da stack!\n");
            exit(1);
        }
        stack = new_stack;
    }
    stack[++top] = strdup(word);
    if (!stack[top]) {
        fprintf(stderr, "Erro de memória no strdup!\n");
        exit(1);
    }
}

void pop() {
    if (top >= 0) {
        free(stack[top--]);
    }
}

char* peek() {
    return (top >= 0) ? stack[top] : "";
}

void free_stack() {
    while (top >= 0) pop();
    free(stack);
}

int main() {
    char line[16384];
    stack = malloc(capacity * sizeof(char*));
    if (!stack) {
        fprintf(stderr, "Erro de alocação inicial da stack!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), stdin)) {
        int popCount = 0;
        top = -1;

        char *token = strtok(line, " \n");
        char **tempStack = malloc(capacity * sizeof(char*));
        if (!tempStack) {
            fprintf(stderr, "Erro de alocação da tempStack!\n");
            free_stack();
            return 1;
        }
        int tempTop = -1;

        while (token) {
            while (top >= 0 && strcmp(token, peek()) < 0) {
                tempStack[++tempTop] = strdup(peek());
                if (!tempStack[tempTop]) {
                    fprintf(stderr, "Erro de memória no strdup tempStack!\n");
                    free_stack();
                    for (int i = 0; i <= tempTop; i++) free(tempStack[i]);
                    free(tempStack);
                    return 1;
                }
                pop();
                popCount++;
            }

            if (popCount == 1)
                printf("push-%s 1x-pop ", token);
            else if (popCount > 1)
                printf("push-%s %dx-pop ", token, popCount);
            else
                printf("push-%s ", token);

            push(token);

            while (tempTop >= 0) {
                push(tempStack[tempTop]);
                printf("push-%s ", tempStack[tempTop]);
                free(tempStack[tempTop--]);
            }

            popCount = 0;
            token = strtok(NULL, " \n");
        }

        printf("\n");
        free(tempStack);
    }

    free_stack();
    return 0;
}
