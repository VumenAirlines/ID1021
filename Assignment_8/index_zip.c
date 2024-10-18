#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>

#define AREAS 10000
#define BUFFER 200
#define AREA_SIZE 100000
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
    area *areas;
    int n;
} codes;
area **read_postcodes(char *file) {
    area **areas = (area**)malloc(sizeof(area*)*AREA_SIZE);
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
        area *a = (area*)malloc(sizeof(area));
        
        char *zip = strtok(copy, ",");
        a->zip = atoi(zip)*100 + atoi(zip+3);
        a->name = strtok(NULL, ",");
        a->pop = atoi(strtok(NULL, ","));
        // update the array of areas
        areas[a->zip] = a;
        a=NULL;
        k++;
    }
    fclose(fptr);
    return areas;
}
int lookup_linear(area **areas, int val){
   
    return areas[val] != NULL ;
}
long bench_lookup_linear(int val,int loop) {
       
    struct timespec t_start, t_stop;
    area **test = read_postcodes("./postnummer.csv");
    int sum=0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        sum += lookup_linear(test,val);
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
        long wall = bench_lookup_linear(98499,loop);       
        med[i] = wall;
    }
    int h = sizeof(med) / sizeof(med[0]);

    qsort(med,h,sizeof(long),compare);
    printf("%s, %s, %0.2f\n", "key indexed", "98499", (double)med[5]/loop); 
    

   return 0;
}