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
        
        char *zip = strtok(copy, ",");
        a.zip = atoi(zip)*100 + atoi(zip+3);
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

void collisions(codes *postnr, int mod) {
    int mx = 20;
    int data[mod];
    int cols[mx];
    for(int i = 0; i < mod; i++) {
        data[i] = 0;
    }
    for(int i = 0; i < mx; i++) {
        cols[i] = 0;
    }
    for (int i = 0; i < postnr->n; i++) {
        int index = (postnr->areas[i].zip)%mod;
        data[index]++;
    }
    int sum = 0;
    for(int i = 0; i < mod; i++) {
        sum += data[i];
        if (data[i] < mx)
        cols[data[i]]++;
    }
    printf("%d (%d) : ", mod, sum);
    for (int i = 1; i < mx; i++) {
        printf("%6d ",cols[i]);
    }
    printf("\n");
}

       
int main() {
    codes *test = read_postcodes("./postnummer.csv");
    collisions(test, 10000);
    collisions(test, 20000);
    collisions(test, 13513);
    collisions(test, 13600);
    collisions(test, 14000);

   return 0;
}