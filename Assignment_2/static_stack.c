
#include <stdlib.h>
#include <stdio.h>

typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

stack *new_stack(int size) {
    int *array = (int*)malloc(size*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    
    stk->top = 0; 
    stk->size = size; 
    stk->array = array; 

    return stk;
}

void push(stack *stk, int val) {
    if(stk->top > stk->size){
        printf("Out of bounds\n");
        return;
    } 
    stk->array[stk->top++] = val;
}

int pop(stack *stk) {
    if (stk->top < 1){ 
        printf("Stack underflow\n");
        return INT_MIN;
    }
     
    return stk->array[--stk->top]; 
}

int main() {
    stack *stk = new_stack(4);
    int n = 10;
    for(int i = 0; i < n; i++) {
        push(stk, i+30);
    }
    for(int i = 0; i < stk->top; i++) {
        printf("stack[%d] : %d\n", i, stk->array[i]);
    }
    int val = pop(stk);
    while(val > INT_MIN) { 
        printf("pop : %d\n", val);
        val = pop(stk);
    }
}
