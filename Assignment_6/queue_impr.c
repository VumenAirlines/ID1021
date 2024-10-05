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

typedef struct node {
    int value;
    struct node *next;
} node;

typedef struct queue {
    node *first;
    node* last;
} queue;


queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    return q;
}

void free_queue(queue *q) {
    node *nxt = q->first;
    while (nxt != NULL) {
        node *tmp = nxt->next;
        free(nxt);
        nxt = tmp;
    }
    free(q);
}
void enqueue(queue* q, int v) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = v;
    nd->next = NULL;
    
    if (q->last != NULL) {
        q->last->next=nd;
    } else {
        q->first=nd;
    }
    q->last = nd;
}
int dequeue(queue *q) {
    int res = 0;
    if (q->first != NULL) {
        res = q->first->value;
        node* nd = q->first;
        q->first = nd->next;
        free(nd);
    }
    return res;
}

int empty(queue *q) {
    return (q->first == NULL);
}

long bench_deq(int n,int loop) {
    struct timespec t_start, t_stop;
    queue *test = create_queue();
    for (int i = 1; i < n; i++)
    {
        enqueue(test,i);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
      for (int j = 0; j < n-1; j++)
      {
        dequeue(test);
      }
            
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free_queue(test);
    
    return wall;
}
long bench_enq(int n,int loop) {
    struct timespec t_start, t_stop;
    queue *test = create_queue();
    for (int i = 1; i < n; i++)
    {
        enqueue(test,i);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
     
        enqueue(test,1);
      
            
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free_queue(test);
    
    return wall;
}



int main() {
    int k = 10;
    int loop =1000;
    long med[k];
    int n = 1;
    int stop=250;//500;
    for (int i = 0; i < stop; i++) {           
        for (int i = 0; i < k; i++) {            
            long wall = bench_enq(n,loop);
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]/1000); 
        n+=10;
    }
  
   return 0;
}