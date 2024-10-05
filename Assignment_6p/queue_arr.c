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


typedef struct queue {
   int first,last,size;
   int* arr;
} queue;


queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
     q->size = 4;
     q->arr = (int*) malloc(sizeof(int)*q->size);
    q->first = 0;
    q->last = 0;
   
    return q;
}

void free_queue(queue *q) {
    free(q->arr);
    free(q);
}
void enqueue(queue* q, int v) {
    q->arr[q->last] = v; //put our value in
    q->last++;//increment last
    if(q->last == q->size) q->last=0; //if we reached the size limit set last to the beginning
    if(q->last==q->first){ //if we wrapped around we need to create a new array
        int* tmp = (int*) malloc(sizeof(int)*2*q->size);
        int tmpindex=0;
        for (int i = q->first; i < q->size; i++)
        {
            tmp[tmpindex++]=q->arr[i];
        }
        for (int i = 0; i < q->last; i++)
        {
            tmp[tmpindex++]=q->arr[i];
        }
        q->size*=2;
        q->first=0;
        q->last=tmpindex;
        free(q->arr);
        q->arr=tmp;     
   }   
}
int dequeue(queue *q) {
    int res = 0;
    if(q->first != q->last){
        res=q->arr[q->first];
        q->first++;
        if(q->first == q->size) q->first=0;
    }
    return res;
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
    int n = 2;
    int stop=16;//500;
    for (int i = 0; i < stop; i++) {           
        for (int i = 0; i < k; i++) {            
            long wall = bench_enq(n,loop);
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]); 
        n*=2;
    }
  
    /*queue *test = create_queue();
    for (int i = 1; i < 11; i++)
    {
        enqueue(test,i);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%d,",test->arr[i]);
    }
    for (int i = 0; i < 10; i++)
    {
       int val = dequeue(test);
        printf("%d,",val);
    }
    printf("%d\n",test->first);
    printf("%d\n",test->last);
    printf("%d\n",test->size);
    for (int i = 2; i < 22; i+=2)
    {
        enqueue(test,i);
    }
     for (int i = 0; i < test->size; i++)
    {
        printf("%d,",test->arr[i]);
    }
     printf("%d\n",test->size);*/
   return 0;
}