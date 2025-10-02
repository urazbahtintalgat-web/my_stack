#include "calculate.h"

#include "my_stack.h"

#include <stdio.h>

#define COMMAND_MAX_LEN 5

CalculateErr calculate(struct Stack * stk, calculate_error_storage_type * err) {
    while (1) {
        char command[COMMAND_MAX_LEN + 1] = "";
        scanf("%s", command);

        CalculateCommands x = choose_command(command);
        if (x == HLT) {
            break;
        }

        switch (x)
        {
        case PUSH:
            int y = 0;
            scanf("%d", &y);
            calculate_push(stk, y, err);
            break;

        case ADD:
            calculate_add(stk, err);
            break;

        case SUB:
            calculate_sub(stk, err);
            break;

        case MUL:
            calculate_mul(stk, err);
            break;

        case DIV:
            calculate_div(stk, err);
            break;

        case OUT:
            calculate_out(stk, err);
            break;
        
        default:
            break;
        }

    }
}

CalculateErr pop_two_elements(struct Stack * stk, stack_type * x, stack_type * y, calculate_error_storage_type * err = NULL) {
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = (CalculateErr) StackPop(stk, x, err)) {
        return possible_error;
    }
    if (possible_error = (CalculateErr) StackPop(stk, y, err)) {
        return possible_error;
    }
}


CalculateErr calculate_push(struct Stack * stk, stack_type value, calculate_error_storage_type * err = NULL) {
    int x = 0;
    scanf("%d", &x);
    return (CalculateErr) StackPush(stk, x, err);
}

CalculateErr calculate_add(struct Stack * stk, calculate_error_storage_type * err = NULL) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = pop_two_elements(stk, &x, &y, err)) {
        return possible_error;
    }
    
    return (CalculateErr) StackPush(stk, x + y, err);
}

CalculateErr calculate_sub(struct Stack * stk, calculate_error_storage_type * err = NULL) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = pop_two_elements(stk, &x, &y, err)) {
        return possible_error;
    }
    
    return (CalculateErr) StackPush(stk, x - y, err);
}

CalculateErr calculate_mul(struct Stack * stk, calculate_error_storage_type * err = NULL) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = pop_two_elements(stk, &x, &y, err)) {
        return possible_error;
    }
    
    return (CalculateErr) StackPush(stk, x * y, err);
}

CalculateErr calculate_div(struct Stack * stk, calculate_error_storage_type * err = NULL) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = pop_two_elements(stk, &x, &y, err)) {
        return possible_error;
    }
    
    if (y == 0) {
        printf("error x / 0\n");
        return WAS_ERROR;
    }
    return (CalculateErr) StackPush(stk, x / y, err);
}

CalculateErr calculate_out(struct Stack * stk, calculate_error_storage_type * err = NULL) {
    stack_type x;
    CalculateErr possible_error = NO_ERROR;
    if (possible_error = (CalculateErr) StackPop(stk, &x, err)) {
        return possible_error;
    }
    printf("%d\n", x);///зависит от stack_type
    return NO_ERROR;
}