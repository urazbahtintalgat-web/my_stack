#include "my_stack.h"

StackErr_t StackInit(stack_t * stk, int capacity, struct StackErr_t_data * err) {
    
    STACK_OK(err, stk);
    /*
    
    
    asserts
    
    
    */
    stk->data = (stack_type*) calloc(capacity, sizeof(stack_type));
    stk->size = 0;
    stk->capacity = capacity;
    /*
     
     
    asserts
    
    
    */
    STACK_OK(err, stk);

    return NO_ERROR;
}

StackErr_t StackPush(stack_t * stk, stack_type value, struct StackErr_t_data * err) {

    STACK_OK(err, stk);
    /*

    
    asserts
    
    
    */
    if (stk->size >= stk->capacity) {
        stk->capacity *= 2;
        stk->data = (stack_type*) realloc(stk->data, stk->capacity * sizeof(stack_type));
    }
    stk->data[stk->size++] = value;
    /*
    
    
    asserts
    
    
    */
    STACK_OK(err, stk);

    return NO_ERROR;
}

stack_type StackPop(stack_t * stk, struct StackErr_t_data * err) {

    STACK_OK(err, stk);
    /*
    
    
    asserts
    
    
    */
    stack_type ans = stk->data[--stk->size];
    stk->data[stk->size] = (stack_type) POISON;
    /*
    
    
    asserts
    
    
    */
    STACK_OK(err, stk);

    return ans;
}

void print_stack_error(struct StackErr_t_data * err) {
    if (err[0].error == NO_ERROR) {
        printf("There is no errors\n");
    } else {
        for (int i = 0; i < ERROR_COUNT; i++) {
            printf("ERROR %d: in file %s\nin function %s\n in line %d\n\n",
                    err[i].error, err[i].file_name, err[i].function_name, err[i].line);
        }
    }
}