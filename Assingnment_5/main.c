#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>


long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
            (t_stop->tv_sec - t_start->tv_sec)*1000000000;
} 
int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

linked *create_linked() {
    linked *new = (linked *)malloc(sizeof(linked));
    if (!new)  return NULL;
    
    new->first = NULL;
    return new;
}

void linked_free(linked *lnk) {
    if (lnk == NULL) return;
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}


void linked_add(linked *lnk, int item) {
    cell *new_cell = (cell *)malloc(sizeof(cell));
    if (!new_cell) return;        
    new_cell->value = item;
    new_cell->tail = lnk->first;
    lnk->first = new_cell;
}

int linked_length(linked *lnk) {
    int count = 0;
    cell *current = lnk->first;
    while (current != NULL) {
        count++;
        current = current->tail;
    }
    return count;
}

bool linked_find(linked *lnk, int item) {
    cell *current = lnk->first;
    while (current != NULL) {
        if (current->value == item) return true;
        current = current->tail;
    }
    return false;
}

void linked_remove(linked *lnk, int item) {
    cell *current = lnk->first;
    cell *previous = NULL;
    while (current != NULL) {
        if (current->value == item) {
            if (previous == NULL) lnk->first = current->tail;         
            else previous->tail = current->tail;
            free(current);
            return;
        }
        previous = current;
        current = current->tail;
    }
}

void linked_append(linked *a, linked *b) {
    if (a == NULL || b == NULL) return;
    if (a->first == NULL) {
        a->first = b->first;
        return;
    }
    cell *current = a->first;
    while (current->tail != NULL) current = current->tail;
    current->tail = b->first;
}


linked *init_list(int n) {
    linked *a = create_linked();
    for (int i = 0; i < n; i++) linked_add(a, i); 
    return a;
}


long bench_a(int n) {
    struct timespec t_start, t_stop;
    linked *a = init_list(n);
    linked *b = init_list(500);

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    linked_append(a, b);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    linked_free(a); 
    free(b);
    
    return wall;
}

long bench_b(int n) {
    struct timespec t_start, t_stop;
    linked *a = init_list(500);
    linked *b = init_list(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    linked_append(a, b);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    linked_free(a); 
    free(b);    
    return wall;
}

int main() {
   bench_b(10000000);
    int k = 10;
    long med[k];
    int n = 1;
    int stop=500;//500;
    for (int i = 0; i < stop; i++) {           
        for (int i = 0; i < k; i++) {            
            long wall = bench_b(n);
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]/1000); 
        n+=50;
    }
   
   return 0;
}