// Equipe: DANIEL SANTANA LIMA e MAURICIO SANTANA DA SILVA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_LISTS 100
#define MAX_NUMS 100

typedef struct {
    int nums[MAX_NUMS];
    int count;
    int sum;
} Lista;

int compare(const void *a, const void *b) {
    Lista *la = (Lista *)a;
    Lista *lb = (Lista *)b;
    return lb->sum - la->sum; // decrescente por soma
}

int main() {
    Lista listas[MAX_LISTS];
    int list_count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), stdin)) {
        char *token = strtok(line, " \n");
        while (token) {
            if (strcmp(token, "start") == 0) {
                Lista nova;
                nova.count = 0;
                nova.sum = 0;
                token = strtok(NULL, " \n");
                while (token && strcmp(token, "start") != 0) {
                    int val = atoi(token);
                    nova.nums[nova.count++] = val;
                    nova.sum += val;
                    token = strtok(NULL, " \n");
                }

                // Remove listas anteriores com mesma soma
                for (int i = 0; i < list_count; i++) {
                    if (listas[i].sum == nova.sum) {
                        listas[i] = listas[--list_count];
                        i--;
                    }
                }

                // Ordena crescente
                for (int i = 0; i < nova.count - 1; i++) {
                    for (int j = i + 1; j < nova.count; j++) {
                        if (nova.nums[i] > nova.nums[j]) {
                            int tmp = nova.nums[i];
                            nova.nums[i] = nova.nums[j];
                            nova.nums[j] = tmp;
                        }
                    }
                }

                listas[list_count++] = nova;
            } else {
                token = strtok(NULL, " \n");
            }
        }
    }

    qsort(listas, list_count, sizeof(Lista), compare);

    for (int i = 0; i < list_count; i++) {
        printf("start");
        for (int j = 0; j < listas[i].count; j++) {
            printf(" %d", listas[i].nums[j]);
        }
        printf("\n");
    }

    return 0;
}
