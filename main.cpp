#include "my_stack.h"
#include <stdio.h>

int main() {
    //unsigned long long err = 0;
    StackErrData err[ERROR_COUNT] = {};


    stack_t stk1 = {};
    StackInit(&stk1, 10, err);
    print_stack_error(err);
    for (int i = 0; i < ERROR_COUNT; i++){
        printf("err%d    %d\n", i, err[i].error);
    }
    StackPush(&stk1, 10);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);
    printf("%d", StackPop(&stk1));
    printf("%d", StackPop(&stk1));
    printf("%d", StackPop(&stk1));
}