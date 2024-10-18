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



typedef struct codes {
    area **areas;
    int max;
    int n;
} codes;

area** create_arr(int size){
    area **tmp = (area**)malloc(sizeof(area*)*size);
     for(int i = 0; i < size; i++) {
        tmp[i]=NULL;
    }
    return tmp;
}
codes *read_postcodes(char *file, int mod) {
    codes *postnr = (codes*)malloc(sizeof(codes)); 
    postnr->max=mod;
    postnr->n=0;
    postnr->areas=create_arr(mod);
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");
    
    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;
    while( (getline(&lineptr, &n, fptr) > 0)) {
        char *copy = (char*)malloc(sizeof(char)*n);
        strcpy(copy, lineptr);
        if (n != BUFFER ) {
        // the line was longer than BUFFER
            free(lineptr);
            n = BUFFER;
        }
        // divide the copy into three strings
        area* a=(area*)malloc(sizeof(area));
        
        char *zip = strtok(copy, ",");
        a->zip = atoi(zip)*100 + atoi(zip+3);
        a->name = strtok(NULL, ",");
        a->pop = atoi(strtok(NULL, ","));
        // update the array of areas
        int idx = a->zip % mod;
        while(postnr->areas[idx] != NULL) {
            idx = (idx + 1) % mod;
            if (idx == a->zip % mod) break;
        }
        postnr->areas[idx] = a;
        postnr->n++;
        
    }
    fclose(fptr);
    return postnr;
}

int lookup(codes* postnr, int val) {
    int idx = val % postnr->max; 
    int sum=0;
    while(postnr->areas[idx] != NULL) {
        sum++;
        if(postnr->areas[idx]->zip == val) {
            printf("%d ",sum);
            return 1;
        }
        idx = (idx + 1) % postnr->max;
        if (idx == val % postnr->max) return 0;
    }
    return 0;
}

       
long bench_lookup(int val,int loop,int size) {
       
    struct timespec t_start, t_stop;
    codes *test = read_postcodes("./postnummer.csv",size);
    int sum=0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < 1; i++)
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
    bench_lookup(98499,10000,60000);
    int k = 10;
    int loop = 10000;
    long med[k];
    int n = 13600;
    int stop = 20;
    for (int i = 0; i < stop; i++) {           
       for (int i = 0; i < k; i++) {            
            long wall = bench_lookup(11115,loop,n);       
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d, %s, %s, %0.2f\n", n, "impr", "11115", (double)med[5]/loop); 
        n+=5000;
    }
 
    
    


   return 0;
}