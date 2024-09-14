#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
            (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}


int unsorted_search(int array[], int length, int key){
    for (int index = 0; index < length ; index++){
        if (array[index] == key){
            return 1;
        }
    }
    return 0;
}
int *sorted(int n) {
    int *array = (int*)malloc(n*sizeof(int));
    int nxt = 0;
    for (int i = 0; i < n ; i++) {
        nxt += rand()%10 + 1;
        array[i] = nxt;
    }
    return array;
}
int sorted_search(int array[], int length, int key){
    for (int index = 0; index < length ; index++){        
        if (array[index] == key) return 1;
        
        if (array[index] > key) return 0;
    }
    return 0;
}
int binary_search(int array[], int length, int key) {
    int first = 0;
    int last = length - 1;

    while (1) {
        int index = (last + first) / 2;
        if (array[index] == key) return 1;

        if (array[index] < key && index < last) {
            first = index + 1;
            continue;
        }

        if (array[index] > key && index > first) {
            last = index;
            continue;
        }
        return 0;
    }
}

int binary_search_recursive(int array[], int length, int key, int first, int last) {
    if (first == last) return 0;
    int mid = (first + last)/2;
    if (array[mid] == key) return 1;
    if (array[mid] < key) return binary_search_recursive(array, length, key, mid + 1, last);
    else return binary_search_recursive(array, length, key, first, mid);
    
}

long bench(int n, int loop) {
    struct timespec t_start, t_stop;
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = rand()%(n*2);
    qsort(array,n,sizeof(int),compare);

    int *keys = (int*)malloc(loop*sizeof(int));
    for (int i = 0; i < loop; i++) keys[i] = rand()%(n*2);
    int sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) {
        int key = keys[i];
        if(unsorted_search(array,n,key)) sum++;
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free(array);
    free(keys);
    return wall;
}

int main() {
   
    int k = 10;
    int loop = 1000;
    long med[k];
    int n = 5000;
    int stop=200;//500;
    for (int i = 0; i < stop; i++) {           
        for (int i = 0; i < k; i++) {            
            long wall = bench(n,loop);
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]/loop/1000); 
        n+=500;
    }
}


