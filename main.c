#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
return (t_stop->tv_nsec - t_start->tv_nsec) +
(t_stop->tv_sec - t_start->tv_sec)*1000000000;
}
int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}
/* seq access with a few elements
int main() {
    struct timespec t_start, t_stop;
    int array[] = {0,1,2,3,4,5,6,7,8,9};
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        sum += array[i];
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
    }
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%ld ns\n", wall);
}*/

/*random access with more elements
int main() {
    struct timespec t_start, t_stop;
    int array[] = {0,1,2,3,4,5,6,7,8,9};
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < 1000; i++) {
        sum += array[rand()%10];
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%ld ns\n", wall/1000);
}*/

/*better random access with mean
long bench(int n, int loop) {
    struct timespec t_start, t_stop;

    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = i;

    int *indx = (int*)malloc(loop*sizeof(int));
    for (int i = 0; i < loop; i++) indx[i] = rand()%n;

    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) sum += array[indx[i]];
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long wall = nano_seconds(&t_start, &t_stop);
    if(sum==0) return sum;
    return wall;
}

*/
/* seq search
long search(int n, int loop) {
    struct timespec t_start, t_stop;
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = rand()%(n*2);

    int *keys = (int*)malloc(loop*sizeof(int));
    for (int i = 0; i < loop; i++) keys[i] = rand()%(n*2);
    int sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) {
        int key = keys[i];
        for (int j = 0; j < n; j++) {
            if (key == array[j]) {
                sum++;
                break;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}*/

// duplicate search
long duplicates(int n) {
    struct timespec t_start, t_stop;

    int *array_a = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array_a[i] = rand()%(n*2);

    int *array_b = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array_b[i] = rand()%(n*2);
    
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < n; i++) {
        int key = array_a[i];
        for (int j = 0; j < n; j++) {
            if (key == array_b[j]) {
                sum++;
                break;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}
int main() {
    int k = 10;
    int loop = 10000;
    long mean[k];
    int n = 5000;
    int stop=50;//500;
    for (int i = 0; i < stop; i++) {
       
        long min = LONG_MAX;
        for (int i = 0; i < k; i++) {
            long wall =duplicates(n);//search(n,loop);//bench(n, loop);
            mean[i]=wall;
    }
    int h = sizeof(mean) / sizeof(mean[0]);

    qsort(mean,h,sizeof(long),compare);
    printf("%d %0.2f\n", n, (double)mean[5]/ 1000000);// loop/1000); //loop);
    n+=500;
    }
   // printf("%f\n", wall/1000.0);
    
}


