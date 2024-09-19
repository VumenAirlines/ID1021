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

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


void selection_sort(int arr[], int n) {
    if(n==0) return;
    for (int i = 0; i < n - 1; i++) {
        int candidate = i;       
        for (int j = i +1; j < n; j++) {
            if (arr[j ] < arr[candidate]) candidate = j;
        }
        if (candidate != i) swap(&arr[i], &arr[candidate]);
    }
}

void insertion_sort(int array[], int n){
   if(n==0) return;
    for (int i = 1; i < n; i++) {
        for (int j = i; j > 0 && array[j] < array[j - 1]; j--) 
            swap(&array[j], &array[j - 1]);
        
    }    
}

void merge(int *org, int *aux, int lo, int mid, int hi) {
    for (int k = lo; k <= hi; k++) aux[k] = org[k];
    int i = lo;        
    int j = mid + 1;   
    for (int k = lo; k <= hi; k++) {
        if (i > mid) org[k] = aux[j++];
        else if (j > hi) org[k] = aux[i++];
        else if (aux[i] <= aux[j]) org[k] = aux[i++];
        else org[k] = aux[j++];
    }
}
void sort(int *org, int *aux, int lo, int hi) {
    if (lo != hi){
        int mid = (lo + hi) / 2; 
        sort(org, aux, lo, mid);      
        sort(org, aux, mid + 1, hi);  
        merge(org, aux, lo, mid, hi);
    }
    
    
}
void merge_sort(int *org, int n) {
    if(n==0) return;
    int *aux = (int*)malloc(n * sizeof(int));
    sort(org, aux, 0, n - 1);
}


int partition(int array[], int low, int high) {
    int pivot = array[high]; 
    int i = low - 1; 

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++; 
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]); 
    return (i + 1); 
}


void quick_sort(int array[], int low, int high) {
    if (low < high) {
        int pivot = partition(array, low, high);
        quick_sort(array, low, pivot - 1);   
        quick_sort(array, pivot + 1, high);  
    }
}



long bench(int n, int loop) {
    struct timespec t_start, t_stop;

   int *array = (int*)malloc(n*sizeof(int));
    
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) {
        
        for (int i = 0; i < n; i++) array[i] = rand()%(n*2);

        quick_sort(array,0,n);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free(array);
    return wall;
}
void dispArr(int *array, int n){
    for (int i =0; i <n; i++)
        printf("%d, ", array[i]);
}

int main() {
   
    int k = 10;
    long med[k];
    int n = 50;
    int loop =10;
    int stop=200;//500;
    for (int i = 0; i < stop; i++) {           
        for (int i = 0; i < k; i++) {            
            long wall = bench(n,loop);
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]/loop/1000); 
        n+=50;
    }
   
   return 0;
}


