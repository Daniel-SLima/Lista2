#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 8192       
#define MAX_LISTS 3000     
#define MAX_NUMS 300        

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
                    if (nova.count >= MAX_NUMS) {
                        fprintf(stderr, "Aviso: número máximo por lista excedido, ignorando extras\n");
                        break;
                    }
                    int val = atoi(token);
                    nova.nums[nova.count++] = val;
                    nova.sum += val;
                    token = strtok(NULL, " \n");
                }

                // Remove listas anteriores com mesma soma (busca simples)
                for (int i = 0; i < list_count; i++) {
                    if (listas[i].sum == nova.sum) {
                        listas[i] = listas[--list_count];
                        i--;
                    }
                }

                // Ordena crescente - melhor usar qsort para performance
                qsort(nova.nums, nova.count, sizeof(int), 
                    (int (*)(const void*, const void*))strcmp);

                // Substituir strcmp para int comparador
                int cmp_int(const void *a, const void *b) {
                    int ia = *(int*)a;
                    int ib = *(int*)b;
                    return ia - ib;
                }
                qsort(nova.nums, nova.count, sizeof(int), cmp_int);

                if (list_count >= MAX_LISTS) {
                    fprintf(stderr, "Aviso: número máximo de listas excedido, ignorando extras\n");
                } else {
                    listas[list_count++] = nova;
                }
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
