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
    int id;
} city;
typedef struct connection{
    city *dst;
    int dist;
} connection;
typedef struct map{
    city** cities;
    int size;
} map;
typedef struct path{
    city* cty;
    city* prev;
    int dist;
} path;
typedef struct done{
    path** arr;
    int n;
} done;

city* create_city(wchar_t* name, int id){
    city* cty = (city*)malloc(sizeof(city));
    cty->n=0;
    cty->size=1;
    cty->con=(connection**)malloc(sizeof(connection*));
    cty->name=name;
    cty->id=id;
    return cty;
}
connection* create_con(city *dst, int dist){
    connection* tmp =(connection*)malloc(sizeof(connection));
    tmp->dist=dist;
    tmp->dst=dst;
    return tmp;
}
path* create_path(city* cty, city* prev, int dist){
     path* tmp = (path*)malloc(sizeof(path));
     tmp->cty=cty;
     tmp->dist=dist;
     tmp->prev=prev;
     return tmp;
}
#pragma region pqueue
typedef struct pqueue{
    path** arr;
    int size;
    int n; 
} pqueue;
pqueue* create_queue(int size){
    pqueue* tmp=(pqueue*)malloc(sizeof(pqueue));
    tmp->arr=(path**)malloc(sizeof(path*)*size);
    tmp->n=0;
    tmp->size=size;
    return tmp;
}
int compare(path* a, path* b){
    if(a->dist==b->dist) return 0;
    if(a->dist>b->dist) return -1;
    if(a->dist<b->dist) return 1;
}
void enqueue(pqueue *q, path *data) {
    int i;
    path *tmp = NULL;
    if (q->n >= q->size) {
        return;
    }

    q->arr[q->n] =  data;
    i = q->n;
    q->n++;

    while(i > 0 && compare(q->arr[i], q->arr[i/2]) > 0) {
        tmp = q->arr[i];
        q->arr[i] = q->arr[i/2];
        q->arr[i/2] = tmp;
        i = i/2;
    }
}
void heapify(pqueue *q, int idx) {
    path *tmp = NULL;
    int left, right, large;
    left =(2 * (idx) + 1);
    right = (2 * (idx) + 2);
    if (left < q->n && compare(q->arr[left], q->arr[idx]) > 0) {
        large = left;
    } else {
        large = idx;
    }
    if (right < q->n && compare(q->arr[right], q->arr[large]) > 0) {
        large = right;
    }
    if (large != idx) {

        tmp = q->arr[large];
        q->arr[large] = q->arr[idx];
        q->arr[idx] = tmp;
        heapify(q, large);
    }
}
path *dequeue(pqueue *q) {
    path *data = NULL;
    if (q->n < 1) {     
         return NULL;     
    }     
    data = q->arr[0];
    q->arr[0] = q->arr[q->n-1];
    q->n--;
    heapify(q, 0);
    return (data);
}

#pragma endregion
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

city* lookup(city** arr, wchar_t* cty,int* id){
    int index = 0;
    if(search(arr, cty,&index)) return arr[index];
    index=hash(cty,MOD);
    while(arr[index] != NULL) {
            index = (index + 1) % MOD;
            if (index ==hash(cty,MOD)) break;
    }
    arr[index]=create_city(cty,(*id)++);
    return arr[index];
}


map *graph(char *file) {
     setlocale(LC_ALL,"en_US.UTF-8");
    int cnt=0;
    
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
        city *src = lookup(cities,wcstok(copy, L",",&cpy),&cnt);
        city *dst = lookup(cities, wcstok(NULL, L",",&cpy),&cnt);
        uint dist = wcstol(wcstok(NULL, L",", &cpy), NULL, 10);
         // printf("%S, %S ,%d \n",src->name,dst->name,dist);
    // add the connection
        connect(src, dst, dist);
        connect(dst, src, dist);
    }
    fclose(fptr);
    trains->size=cnt;
    return trains;
}

int dijkstra(map* map, wchar_t* arg1, wchar_t* arg2, int* size){
    int a=0;
    if(!search(map->cities,arg2,&a)||!search(map->cities,arg1,&a)) return -1;
    city *src = lookup(map->cities, arg1,NULL);
    city *dst = lookup(map->cities, arg2,NULL);
    pqueue *q = create_queue(1000);
    done* don=(done*)malloc(sizeof(done));
    don->arr=(path**)malloc(sizeof(path*)*1000);
    don->n=0;
    for (int i = 0; i < 1000; i++)
    {
         don->arr[i]=NULL;
    }
    enqueue(q,create_path(lookup(map->cities,src->name,NULL),NULL,0));
    path* tmp;
    while (q->n>0)
    {
        tmp=dequeue(q);
        if(don->arr[tmp->cty->id]!=NULL) continue;
        don->arr[tmp->cty->id]=tmp;
        don->n++;
        if(tmp->cty->id==dst->id) break;
        for (int i = 0; i < tmp->cty->n; i++)
        {
            path* p=create_path(lookup(map->cities,tmp->cty->con[i]->dst->name,NULL),lookup(map->cities,tmp->cty->name,NULL),tmp->dist+tmp->cty->con[i]->dist);
           
            if(don->arr[p->cty->id]==NULL)
                enqueue(q, p);
            
        }
        
    }
    *size=don->n;
    return don->arr[tmp->cty->id]->dist;
    
}

int main(int argc, char** argv) {
    setlocale(LC_ALL,"en_US.UTF-8");    
    map *trains = graph("europe.csv");
   
    wchar_t *arg1 = (wchar_t *)malloc((mbstowcs(NULL, argv[1], 0) + 1) * sizeof(wchar_t));
    wchar_t *arg2 = (wchar_t *)malloc((mbstowcs(NULL, argv[2], 0) + 1) * sizeof(wchar_t));
    mbstowcs(arg1, argv[1], (mbstowcs(NULL, argv[1], 0) + 1));
    mbstowcs(arg2, argv[2], (mbstowcs(NULL, argv[2], 0) + 1));
    //city *from = lookup(trains->cities, arg1,NULL);
   // city *to = lookup(trains->cities, arg2,NULL);

 
   int n=0;
    struct timespec t_start, t_stop;

    clock_gettime(CLOCK_MONOTONIC, &t_start);
   int s=dijkstra(trains,arg1,arg2,&n);
       
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long wall = nano_seconds(&t_start, &t_stop);
      

    if (s > 0)
        printf("%S  %S  %d  %.2f  %d\n",arg1,arg2, s, ((double)wall)/1000,n);
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