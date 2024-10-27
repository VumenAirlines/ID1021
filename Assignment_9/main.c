#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <wchar.h>
#include <locale.h>

#define MOD 13700
#define BUFFER 256

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
int hash(wchar_t *name, int mod) {
    int h = 0;
    int i = 0;
    wchar_t c = 0;
    while((c = name[i]) != 0) {
        h = (h*31 + c) % mod;
        i++;
    }
    return h;
}



typedef struct city
{
    wchar_t* name;
    struct connection** con;
    int n;
    int size;
} city;
typedef struct connection{
    city *dst;
    int dist;
} connection;
typedef struct map{
    city** cities;
   
} map;

city* create_city(wchar_t* name){
    city* cty = (city*)malloc(sizeof(city));
    cty->n=0;
    cty->size=1;
    cty->con=(connection**)malloc(sizeof(connection*));
    cty->name=name;
    return cty;
}
connection* create_con(city *dst, int dist){
    connection* tmp =(connection*)malloc(sizeof(connection));
    tmp->dist=dist;
    tmp->dst=dst;
    return tmp;
}

void connect(city* src, city* dst, int dist){
    if(src->n==src->size){
        connection** tmp = src->con;
        src->con=(connection**)malloc(sizeof(connection*)*src->size*2);
        src->size *= 2;
        for (int i = 0; i < src->n; i++)
        {
            src->con[i]=tmp[i];
        }
        free(tmp);
    }
    src->con[src->n++]=create_con(dst,dist);
    //printf( "%S, %S \n",src->name,src->con[src->n - 1].dst->name);
}
int search(city** arr, wchar_t* cty, int* index) {
    int idx = hash(cty, MOD); 
    int sum=0;
    while(arr[idx] != NULL) {
        sum++;
        if(wcscmp(arr[idx]->name,cty)==0) {
            *index = idx;
            return 1;
        }
        idx = (idx + 1) % MOD;
        if (idx == hash(cty, MOD)) return 0;
    }
    return 0;
}

city* lookup(city** arr, wchar_t* cty){
    int index = 0;
    if(search(arr, cty,&index)) return arr[index];
    index=hash(cty,MOD);
    while(arr[index] != NULL) {
            index = (index + 1) % MOD;
            if (index ==hash(cty,MOD)) break;
    }
    arr[index]=create_city(cty);
    return arr[index];
}


map *graph(char *file) {
    // buckets or open adressing
    city** cities = (city**)malloc(sizeof(city*) * MOD);
    map *trains = (map*)malloc(sizeof(map));
    trains->cities = cities;
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");
    if (fptr == NULL) return NULL;
    wchar_t *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;
    while(fgetws(lineptr, BUFFER, fptr) != NULL) {
        wchar_t *copy = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(lineptr)+1));
        wcscpy(copy, lineptr);
        wchar_t* cpy;

    // divide the copy into the three parts
        city *src = lookup(cities,wcstok(copy, L",",&cpy));
        city *dst = lookup(cities, wcstok(NULL, L",",&cpy));
        uint dist = wcstol(wcstok(NULL, L",", &cpy), NULL, 10);
         // printf("%S, %S ,%d \n",src->name,dst->name,dist);
    // add the connection
        connect(src, dst, dist);
        connect(dst, src, dist);
    }
    fclose(fptr);
    return trains;
}

int shortest(city *from, city *to, int left) {
    if (from == to) {
        return 0;
    }
    int sofar = -1;
    connection *nxt;
    for (int i = 0; i < from->n; i++){
        nxt = from->con[i];
        if (nxt->dist <= left) {
            int d = shortest(nxt->dst, to, left - nxt->dist);
            if (d >= 0 && ((sofar == -1 ) || (d + nxt->dist) < sofar))
                sofar = (d + nxt->dist);
        }
        
    }
    return sofar;
}
int loop(city** path, city* city, int k) {
    for(int i = 0; i < k; i++) {
        if(path[i] == city) return 1;
    }
    return 0;
}
int shortest_2(city *from, city *to, city** path, int k, int current_length, int* length) {
    if (from == to) {
        if(*length == -1 || current_length < *length) *length = current_length;
        return 0;
    }

    int sofar = -1;
    if(*length >= 0 && current_length > *length) {
        return sofar;
    }

    for (int i = 0; i < from->n; i++) {
        connection *nxt = from->con[i];
        if(!loop(path, nxt->dst, k)) {
            path[k] = nxt->dst;
            int d = shortest_2(nxt->dst, to, path, k + 1, current_length + nxt->dist, length);
            if (d >= 0 && ((sofar == -1) || (d + nxt->dist) < sofar)) {
                sofar = (d + nxt->dist);      
            }
        }
    }
    return sofar;
}

int main(int argc, char** argv) {
    setlocale(LC_ALL,"en_US.UTF-8");    
    map *trains = graph("trains.csv");
    if(argc < 4) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        exit(1);
    }
    wchar_t *arg1 = (wchar_t *)malloc((mbstowcs(NULL, argv[1], 0) + 1) * sizeof(wchar_t));
    wchar_t *arg2 = (wchar_t *)malloc((mbstowcs(NULL, argv[2], 0) + 1) * sizeof(wchar_t));
    mbstowcs(arg1, argv[1], (mbstowcs(NULL, argv[1], 0) + 1));
    mbstowcs(arg2, argv[2], (mbstowcs(NULL, argv[2], 0) + 1));

    city *from = lookup(trains->cities, arg1);
    city *to = lookup(trains->cities, arg2);

    int limit = atoi(argv[3]);
 
    printf("%S, %S, %d, ", from->name,to->name,limit);
    struct timespec t_start, t_stop;
 city** path = (city**)malloc(sizeof(city*)*10000);
      int a = -1;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
   
        int s = shortest_2(from, to, path, 0, 0, &a);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long wall = nano_seconds(&t_start, &t_stop);
    if (s > 0)
        printf(" %d, %.2f\n", s, ((double)wall)/1000000);
    else
        printf("no path found\n");
}

/*
int main() {
    
    int k = 10;
    int loop =100;
    long med[k];
    int n = 2;
    int stop=200;//500;
    for (int i = 0; i < stop; i++) {   
        
        for (int i = 0; i < k; i++) {            
            long wall = bench_lookup(n,loop);
            
            med[i] = wall;
        }
        int h = sizeof(med) / sizeof(med[0]);

        qsort(med,h,sizeof(long),compare);
        printf("%d %0.2f\n", n, (double)med[5]/1000/50); 
        n+=100;
    }

   return 0;
}*/