#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>


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
typedef struct node {
    int value;
    struct node *right;
    struct node *left;
} node;

#pragma region stack

typedef struct stack {
    int top;
    int size;
    int num;
    node **array;
} stack;

stack *new_stack(int size) {
    node **array = (node**)malloc(size*sizeof(node *));
    stack *stk = (stack*)malloc(sizeof(stack));
    stk->num=0;
    stk->top = 0;
    stk->size=size;
    stk->array=array;
    return stk;
}

void push(stack *stk, node* val) {
    stk->num++;
    if (stk->top == stk->size) {
        node **tmp = (node **)malloc(2*stk->size*sizeof(node* ));
        for (int i = 0; i < stk->size; i++) {
            tmp[i] = stk->array[i];
        }

        free(stk->array);
        stk->size *= 2;
        stk->array = tmp;
        stk->array[stk->top++] = val;
    }
    else{
        stk->array[stk->top++] = val;
    } 
}

node * pop(stack *stk) {
    if (stk->top < 1){ 
        printf("Stack underflow\n");
        return NULL;
    }
    stk->num--;
    if(stk->top <= stk->size/4 && stk->size>= 4){
			stk->size  = stk->size/2;
       
			node **tmp = (node **)malloc(stk->size * sizeof(node *));
			for (unsigned i = 0; i < stk->size; i++)
			{
				tmp[i] = stk->array[i];
			}
			free(stk->array);
			stk->array = tmp;
    }
    return stk->array[--stk->top]; 
}

#pragma endregion


typedef struct tree {
    node *root;
} tree;

tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
}

node *construct_node(int val) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}
void free_node(node *nd) {
    if (nd != NULL) {
        free(nd->right);
        free(nd->left);
        free(nd);
    }
}
void free_tree(tree *tr) {
    if(tr==NULL) return;
    if(tr->root->left!=NULL){
        tr->root=tr->root->left;
        free_tree(tr);
    }
    if(tr->root->right!=NULL){
        tr->root=tr->root->right;
        free_tree(tr);
    }
    if(tr->root==NULL){}
        free(tr);
}
void add_node(node *root, int value){
    if (root->value == value) return;
    if (value < root->value) {
        if (root->left == NULL) {
            root->left = construct_node(value);
           
        } 
        else {
            add_node(root->left, value);
        }
    } 
    else {
        if (root->right == NULL) {
            root->right = construct_node(value);
        }
         else {
            add_node(root->right, value);
        }
    }
}
void add_rec(tree *tr, int value){  
    if(tr->root == NULL) 
        tr->root = construct_node(value);
    else add_node(tr->root, value);
    

}
void add_it(tree *tr, int value){  
    if(tr==NULL) return;
              
    if(tr->root==NULL){
        
        tr->root=construct_node(value); 
        return;
    }
         
    node *tmp = tr->root; 
    node *tmp1 = NULL;
    while(tmp != NULL){
        if(tmp->value == value) return;
        tmp1=tmp;
        if(tmp->value > value)
            tmp=tmp->left;
        else
            tmp=tmp->right;
    }
    if(tmp1->value>value) tmp1->left=construct_node(value);
    else tmp1->right=construct_node(value);
    

}
int lookup(node *root, int value){
     if(root==NULL){     
        return 0;      
    }
        
    if(root->value==value){      
        return 1;
    }
       
    if(root->value > value){      
       return lookup(root->left,value);   
    }
    else{       
       return lookup(root->right,value);    
    }
}

void print(tree *tr) {
    stack *stk = new_stack(4);
    node *cur = tr->root;
    while(cur!=NULL){
        push(stk,cur);
       
        cur=cur->left;
    }
  
    while(stk->num!=0) {

        cur=pop(stk);
        printf("%d, ", cur->value);
       
        
        if( cur->right != NULL) {
            cur=cur->right; 
            while(cur!=NULL){
              
                push(stk,cur);
                cur=cur->left;
            }               
        }         
       
    }
       
}
void print_tree(node* nd) {
    if (nd == NULL) return;      
    print_tree(nd->left);
    printf("%d ", nd->value);
    print_tree(nd->right);   
}
long bench_add_rec(int n,int loop) {
       
    struct timespec t_start, t_stop;
    tree *test = construct_tree();
    for (int i = 0; i < n; i++)
    {
        add_rec(test,rand()%(n+10));
    }
  
  
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        add_rec(test,rand()%(2*n+10)+1);
    }
   

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long wall = nano_seconds(&t_start, &t_stop);
    //free_tree(test);
    return wall;
}

long bench_add_it(int n,int loop) {
       
    struct timespec t_start, t_stop;
    tree *test = construct_tree();
   // for (int i = 0; i < n; i++)
  //  {
         add_it(test,rand()%(n+10));
   // }
   // print_tree(test->root);
    //printf("\n");
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
         add_it(test,rand()%(n+10));
    }
            
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long wall = nano_seconds(&t_start, &t_stop);
    //free_tree(test);
    return wall;
}
long bench_lookup(int n,int loop) {
       
    struct timespec t_start, t_stop;
    tree *test = construct_tree();
    for (int i = 0; i < n; i++)
    {
        add_rec(test,i );
    }
    //print_tree(test->root);
    //printf("\n");
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < 50; i++)
    {
        //printf("%d\n",test->root->value);
       int res = lookup(test->root,rand() % (n+1));
    }
            
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long wall = nano_seconds(&t_start, &t_stop);
    //free_tree(test);
    return wall;
}


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
}