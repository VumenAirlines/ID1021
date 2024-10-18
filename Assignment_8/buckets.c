#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>

#define AREAS 10000
#define BUFFER 200

#pragma region asd
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
#pragma endregion

typedef struct area {
    int zip;
    char *name;
    int pop;
} area;


typedef struct bucket {
    int size;
    int max;
    area *areas;
} bucket;
typedef struct codes {
    bucket* buckets;
    int n;
} codes;
bucket* create_buckets(int size){
    bucket *tmp = (bucket*)malloc(sizeof(bucket)*size);
     for(int i = 0; i < size; i++) {
        tmp[i].areas = (area*)malloc(sizeof(area));
        tmp[i].size = 0;
        tmp[i].max = 1;
    }
    return tmp;
}

codes *read_postcodes(char *file, int mod) {
    codes *postnr = (codes*)malloc(sizeof(codes)); 
    bucket *buckets = create_buckets(mod);
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");
    int k = 0;
    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;
    while((k < AREAS) && (getline(&lineptr, &n, fptr) > 0)) {
        char *copy = (char*)malloc(sizeof(char)*n);
        strcpy(copy, lineptr);
        if (n != BUFFER ) {
        // the line was longer than BUFFER
            free(lineptr);
            n = BUFFER;
        }
        // divide the copy into three strings
        area a;
        
        char *zip = strtok(copy, ",");
        a.zip = atoi(zip)*100 + atoi(zip+3);
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));
        // update the array of areas
        bucket tmp=buckets[a.zip%mod];
        if(tmp.size==tmp.max){
            area* arr = (area*)malloc(sizeof(area)*2*tmp.max);
            for (int i = 0; i < tmp.max; i++)
            {
                arr[i]=tmp.areas[i];
            }
            arr[tmp.size++]=a;
            tmp.max *=2;
            tmp.areas = arr;            
        }
        else  tmp.areas[tmp.size++]=a;
        buckets[a.zip%mod]=tmp;
        k++;
    }
    fclose(fptr);
    postnr->buckets = buckets;
    postnr->n = mod;
    return postnr;
}

int lookup(codes *postnr, int val) {
    for(int i = 0; i < postnr->buckets[val % postnr->n].size; i++) {
        if(postnr->buckets[val % postnr->n].areas[i].zip == val) return 1;
    }
    return 0;
}

       
long bench_lookup(int val,int loop) {
       
    struct timespec t_start, t_stop;
    codes *test = read_postcodes("./postnummer.csv",14000);
    int sum=0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        sum += lookup(test,val);
    }   
    //printf("%d",sum);        
    clock_gettime(CLOCK_MONOTONIC, &t_stop);   
    long wall = nano_seconds(&t_start, &t_stop);
    free(test);
    return wall;
}

int main() {
    
    int k = 10;
    int loop = 10000;
    long med[k];
 
 
    for (int i = 0; i < k; i++) {            
        long wall = bench_lookup(98499,loop);       
        med[i] = wall;
    }
    int h = sizeof(med) / sizeof(med[0]);

    qsort(med,h,sizeof(long),compare);
    printf("%s, %s, %0.2f\n", "buckets", "98499", (double)med[5]/loop); 
    


   return 0;
}