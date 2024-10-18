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
    char *zip;
    char *name;
    int pop;
} area;
typedef struct codes {
    area *areas;
    int n;
} codes;
codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    area *areas = (area*)malloc(sizeof(area)*AREAS);
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
        a.zip = strtok(copy, ",");
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));
        // update the array of areas
        areas[k++] = a;
    }
    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}
int lookup(codes *postnr, char* str){
    for (int i = 0; i < postnr->n; i++)
    {
        area tmp = postnr->areas[i];
        if(strcmp(tmp.zip, str)==0) return 1;
    }
    return 0;
}
long bench_lookup(char* str,int loop) {
       
    struct timespec t_start, t_stop;
    codes *test = read_postcodes("./postnummer.csv");
    int sum=0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        sum += lookup(test,str);
    }           
    clock_gettime(CLOCK_MONOTONIC, &t_stop);   
    long wall = nano_seconds(&t_start, &t_stop);

    return wall;
}
int main() {
    
    int k = 10;
    int loop = 10000;
    long med[k];
 
 
    for (int i = 0; i < k; i++) {            
        long wall = bench_lookup("984 99",loop);       
        med[i] = wall;
    }
    int h = sizeof(med) / sizeof(med[0]);

    qsort(med,h,sizeof(long),compare);
    printf("%s, %s, %0.2f\n", "string", "984 99", (double)med[5]/loop); 
    

   return 0;
}