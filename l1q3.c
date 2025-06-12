// Equipe: DANIEL SANTANA LIMA e MAURICIO SANTANA DA SILVA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 3100

typedef struct FloatNode {
    float val;
    struct FloatNode* next;
} FloatNode;

typedef struct IntNode {
    int val;
    FloatNode* floats;
    struct IntNode* prev;
    struct IntNode* next;
} IntNode;

int float_cmp_desc(const void* a, const void* b) {
    float fa = *(float*)a;
    float fb = *(float*)b;
    return (fb > fa) - (fb < fa);
}

IntNode* insert_sorted(IntNode* head, int val) {
    IntNode* node = malloc(sizeof(IntNode));
    if (!node) {
        fprintf(stderr, "Erro de alocação IntNode\n");
        exit(1);
    }
    node->val = val;
    node->floats = NULL;
    node->prev = node->next = NULL;

    if (!head) return node;

    IntNode* curr = head;
    if (val < curr->val) {
        node->next = head;
        head->prev = node;
        return node;
    }

    while (curr->next && curr->next->val < val)
        curr = curr->next;

    node->next = curr->next;
    node->prev = curr;
    if (curr->next) curr->next->prev = node;
    curr->next = node;
    return head;
}

void add_float(IntNode* node, float f) {
    FloatNode* newf = malloc(sizeof(FloatNode));
    if (!newf) {
        fprintf(stderr, "Erro de alocação FloatNode\n");
        exit(1);
    }
    newf->val = f;
    newf->next = NULL;

    if (!node->floats) {
        newf->next = newf;
        node->floats = newf;
        return;
    }

    FloatNode* curr = node->floats;
    while (curr->next != node->floats)
        curr = curr->next;

    curr->next = newf;
    newf->next = node->floats;
}

void sort_floats_desc(IntNode* node) {
    if (!node->floats || node->floats->next == node->floats) return;

    FloatNode* curr = node->floats;
    int count = 0;
    do {
        count++;
        curr = curr->next;
    } while (curr != node->floats);

    float* arr = malloc(count * sizeof(float));
    if (!arr) {
        fprintf(stderr, "Erro de alocação para sort_floats_desc\n");
        exit(1);
    }

    curr = node->floats;
    for (int i = 0; i < count; i++) {
        arr[i] = curr->val;
        curr = curr->next;
    }

    qsort(arr, count, sizeof(float), float_cmp_desc);

    // Liberar lista antiga
    FloatNode* start = node->floats;
    curr = start->next;
    while (curr != start) {
        FloatNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(start);

    // Recriar lista circular ordenada decrescente
    FloatNode* head = malloc(sizeof(FloatNode));
    if (!head) {
        fprintf(stderr, "Erro de alocação para nova lista floats\n");
        exit(1);
    }
    head->val = arr[0];
    head->next = head;

    FloatNode* last = head;
    for (int i = 1; i < count; i++) {
        FloatNode* fn = malloc(sizeof(FloatNode));
        if (!fn) {
            fprintf(stderr, "Erro de alocação para nova lista floats\n");
            exit(1);
        }
        fn->val = arr[i];
        fn->next = head;
        last->next = fn;
        last = fn;
    }

    node->floats = head;
    free(arr);
}

void print_list(IntNode* head) {
    IntNode* curr = head;
    while (curr) {
        printf("%d(", curr->val);
        FloatNode* f = curr->floats;
        if (f) {
            FloatNode* start = f;
            do {
                printf("%.2f", f->val);
                f = f->next;
                if (f != start) printf("->");
            } while (f != start);
        }
        printf(")");
        if (curr->next) printf("->");
        curr = curr->next;
    }
    printf("\n");
}

void free_all(IntNode* head) {
    while (head) {
        IntNode* temp = head;
        head = head->next;

        if (temp->floats) {
            FloatNode* f = temp->floats->next;
            while (f != temp->floats) {
                FloatNode* next = f->next;
                free(f);
                f = next;
            }
            free(temp->floats);
        }

        free(temp);
    }
}

int main() {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), stdin)) {
        IntNode* head = NULL;
        int le_done = 0;
        char* token = strtok(line, " \n");

        int* ints = malloc(32 * sizeof(int));
        int int_cap = 32, int_count = 0;
        float* floats = malloc(64 * sizeof(float));
        int float_cap = 64, float_count = 0;

        if (!ints || !floats) {
            fprintf(stderr, "Erro de alocação de memória\n");
            exit(1);
        }

        while (token) {
            if (strcmp(token, "LE") == 0) {
                le_done = 0;
            } else if (strcmp(token, "LI") == 0) {
                le_done = 1;
            } else if (!le_done) {
                if (int_count == int_cap) {
                    int_cap *= 2;
                    ints = realloc(ints, int_cap * sizeof(int));
                    if (!ints) { fprintf(stderr,"Erro realocação ints\n"); exit(1);}
                }
                ints[int_count++] = atoi(token);
            } else {
                if (float_count == float_cap) {
                    float_cap *= 2;
                    floats = realloc(floats, float_cap * sizeof(float));
                    if (!floats) { fprintf(stderr,"Erro realocação floats\n"); exit(1);}
                }
                floats[float_count++] = atof(token);
            }
            token = strtok(NULL, " \n");
        }

        for (int i = 0; i < int_count; i++) {
            head = insert_sorted(head, ints[i]);
        }

        for (int i = 0; i < float_count; i++) {
            for (IntNode* n = head; n; n = n->next) {
                if (fabsf(n->val - floats[i]) < 1.0f) {
                    add_float(n, floats[i]);
                    break;
                }
            }
        }

        for (IntNode* n = head; n; n = n->next) {
            sort_floats_desc(n);
        }

        print_list(head);
        free_all(head);
        free(ints);
        free(floats);
    }

    return 0;
}
