#include "my_stack.h"
#include <stdio.h>

int main() {
    //unsigned long long err = 0;
    StackErr_t_data err[ERROR_COUNT] = {};


    stack_t stk1 = {};
    StackInit(&stk1, 10, err);
    for (int i = 0; i < 10; i++){
        printf("err%d    %d\n", i, err[i]);
    }
    StackPush(&stk1, 10);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);
    printf("%d", StackPop(&stk1));
    printf("%d", StackPop(&stk1));
    printf("%d", StackPop(&stk1));
}