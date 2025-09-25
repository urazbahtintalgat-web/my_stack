#include "my_stack.h"


void StackOK(stack_t * stk, struct StackErrData * err, const char file_name[], const char func_name[], const int line) {
    if (err != NULL) {
        if (stk == NULL) {
            STACK_ERROR_SAVE(err, NULL_PTR, file_name, func_name, line)
            err[NO_ERROR].error = WAS_ERROR;
        } else {
            if (stk->data == NULL && strcmp(func_name, "StackInit") != 0) {
                STACK_ERROR_SAVE(err, NULL_DATA, file_name, func_name, line)
                err[NO_ERROR].error = WAS_ERROR;
            }
            if (stk->capacity <= 0) {
                STACK_ERROR_SAVE(err, INVALID_CAPACITY, file_name, func_name, line)
                err[NO_ERROR].error = WAS_ERROR;          
            }
            if (stk->size > stk->capacity) {              
                STACK_ERROR_SAVE(err,SIZE_BIGGER_CAPACITY, file_name, func_name, line)
                err[NO_ERROR].error = WAS_ERROR;
            }
        }
    }
}


StackErr StackInit(stack_t * stk, size_t capacity, struct StackErrData * err) {
    if (capacity <= 0) {
        STACK_ERROR_SAVE(err, INVALID_CAPACITY, __FILE__, __func__, __LINE__)
        printf("Was error\n");
        return WAS_ERROR;
    }

    stk->data = (stack_type*) calloc(capacity, sizeof(stack_type));
    stk->size = 0;
    stk->capacity = capacity;

    if (stk->data == NULL) {
        STACK_ERROR_SAVE(err, ALLOC_FALED, __FILE__, __func__, __LINE__)
        printf("Was error\n");
        return WAS_ERROR;
    }
    
    StackOK(stk, err, __FILE__, __func__, __LINE__);

    return NO_ERROR;
}

StackErr StackPush(stack_t * stk, stack_type value, struct StackErrData * err) {

    StackOK(stk, err, __FILE__, __func__, __LINE__);
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
    StackOK(stk, err, __FILE__, __func__, __LINE__);

    return NO_ERROR;
}

stack_type StackPop(stack_t * stk, struct StackErrData * err) {

    StackOK(stk, err, __FILE__, __func__, __LINE__);
    /*
    
    
    asserts
    
    
    */
    stack_type ans = stk->data[--stk->size];
    stk->data[stk->size] = (stack_type) POISON;
    /*
    
    
    asserts
    
    
    */
    StackOK(stk, err, __FILE__, __func__, __LINE__);

    return ans;
}

void save_error(struct StackErr_t_data * err, char file_name[], char function_name[], int line){

}

void print_stack_error(struct StackErrData * err) {
    if (err[0].error == NO_ERROR) {
        printf("There is no errors\n");
    } else {
        for (int i = 1; i < ERROR_COUNT; i++) {
            if ((int)err[i].error != (int) NO_ERROR){
                printf("ERROR %d: in file %s\nin function %s\n in line %d\n\n",
                        err[i].error, err[i].file_name, err[i].func_name, err[i].line);
            }
        }
    }
}