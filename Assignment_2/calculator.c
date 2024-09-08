
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <string.h>
#include <limits.h>


typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

stack *new_stack() {
    int *array = (int*)malloc(4*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    
    stk->top = 0;
    stk->size=4;
    stk->array=array;
    return stk;
}

void push(stack *stk, int val) {
    if (stk->top == stk->size) {
        int *tmp = (int*)malloc(2*stk->size*sizeof(int));
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

int pop(stack *stk) {
    if (stk->top == 0){ 
        return INT_MIN;
    }
    
    if(stk->top <= stk->size/4 && stk->size> 4){
			stk->size  = stk->size/2;
       
			int *tmp = (int *)malloc(stk->size * sizeof(int));
			for (unsigned i = 0; i < stk->size; i++)
			{
				tmp[i] = stk->array[i];
			}
			free(stk->array);
			stk->array = tmp;
            

    }

    int val = stk->array[--stk->top]; 
    return val;


}


int main(){
    stack *stk=new_stack();
    printf("HP-35 pocket calculator\n");

    size_t n = 10;
    char *buffer = (char*)malloc(n*sizeof(char));

    bool run = true;

    while(run) {
        printf(" > ");
        getline(&buffer, &n, stdin);
        
        if (strcmp(buffer, "\n") == 0){
            run = false;
        } 
        

        else if (strcmp(buffer, "+\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a+b);
        }
        else if (strcmp(buffer, "-\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a-b);
        }
        else if (strcmp(buffer, "*\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a*b);
        }
        else if (strcmp(buffer, "/\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a/b);
        }
        else{
            int val = atoi(buffer);           
            push(stk, val);
        }
    }
    printf("the result is: %d\n", pop(stk));
}
