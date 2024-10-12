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

node *construct_node(int val) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}
#pragma region queue

typedef struct queue {
   int first,last,size,num;
   node** arr;
} queue;


queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->size = 4;
    q->arr = (node **) malloc(sizeof(node * )*q->size);
    q->first = 0;
    q->last = 0;
    q->num =0;
    return q;
}

void free_queue(queue *q) {
    free(q->arr);
    free(q);
}
void enqueue(queue* q, node *v) {
    q->num++;
    q->arr[q->last] = v; //put our value in
    q->last++;//increment last
    if(q->last == q->size) q->last=0; //if we reached the size limit set last to the beginning
    if(q->last==q->first){ //if we wrapped around we need to create a new array
        node** tmp = (node**) malloc(sizeof(node *)*2*q->size);
        int tmpindex=0;
        for (int i = q->first; i < q->size; i++)
        {
            tmp[tmpindex++]=q->arr[i];
        }
        for (int i = 0; i < q->last; i++)
        {
            tmp[tmpindex++]=q->arr[i];
        }
        q->size*=2;
        q->first=0;
        q->last=tmpindex;
        free(q->arr);
        q->arr=tmp;     
   }   
}
node* dequeue(queue *q) {
    node *res = construct_node(-1);
    if(q->first != q->last){
        q->num--;
        res=q->arr[q->first];
        q->first+=1;
        if(q->first == q->size) q->first=0;
    }
    return res;
}
int is_empty(queue *q){
    return q->num == 0;
}
#pragma endregion

#pragma region tree
typedef struct tree {
    node *root;
} tree;

tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
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

void add_rec(node **root, int value){  
    if((*root)==NULL){
        *root=construct_node(value); 
        return;      
    }
        
    if((*root)->value==value){      
        return;
    }
       
    if((*root)->value > value){      
        add_rec(&(*root)->left,value);   
    }
    else{       
        add_rec(&(*root)->right,value);    
    }
    

}
void add_it(tree *tr, int value){  
    if(tr==NULL)    
        return;      
    if(tr->root==NULL)
         tr->root=construct_node(value); 
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
        lookup(root->left,value);   
    }
    else{       
        lookup(root->right,value);    
    }
}

#pragma endregion

typedef struct sequence{
    queue *q;
} sequence;
sequence* create_sequence(tree *tr){
    sequence *seq = (sequence*)malloc(sizeof(sequence *));
    seq->q=create_queue();
    enqueue(seq->q,tr->root);
    return seq;
}
int next(sequence *seq){
    if(is_empty(seq->q)) { printf("%s","empty");return -1;}
    node *tmp=dequeue(seq->q);
    if(tmp==NULL){ printf("%s","empty1");return -1;}
    int val=tmp->value;
    if(tmp->left!=NULL)
        enqueue(seq->q,tmp->left);
    if(tmp->right!=NULL)
        enqueue(seq->q,tmp->right);
   //printf("%d",tmp->value);
    
    return val;
}
void bfs_print(tree *tr){
    queue *q=create_queue();
    node *tmp = tr->root;
    enqueue(q,tmp);
    while(!is_empty(q)){
        tmp=dequeue(q);
        printf("%d ",tmp->value);
        if(tmp->left!=NULL)
            enqueue(q,tmp->left);
        if(tmp->right!=NULL)
            enqueue(q,tmp->right);

    }
}
void print_tree(node* nd) {
    if (nd == NULL) return;      
    print_tree(nd->left);
    printf("%d ", nd->value);
    print_tree(nd->right);   
}


int main() {
  
    tree *test = construct_tree();
    add_it(test,40);
    add_it(test,30);
    add_it(test,50);
    add_it(test,20);
    add_it(test,25);
    add_it(test,100);
    add_it(test,24);
    add_it(test,35);
    add_it(test,23);
    print_tree(test->root);
    printf("\n");
    sequence *ts = create_sequence(test);
    printf("%d\n", next(ts));
    add_it(test,49);
    printf("%d\n", next(ts));
    printf("%d\n", next(ts)); 
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    add_it(test,22);
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    printf("%d\n", next(ts));
    print_tree(test->root);
    printf("\n");
    bfs_print(test);
   return 0;
}