#include "calculate.h"

#include "my_stack.h"

#include <stdio.h>

#define COMMAND_MAX_LEN 5

StackErr calculate(struct Stack * stk, struct StackErrData * err) {
    while (1) {
        char command[COMMAND_MAX_LEN + 1] = {};
        scanf("%s", command);

        CalculateCommands x = choosecommand(command);
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

StackErr calculate_push(struct Stack * stk, int value, struct StackErrData * err = NULL) {
    int x = 0;
    scanf("%d", &x);
    return StackPush(stk, x, err);
}

StackErr calculate_add(struct Stack * stk, struct StackErrData * err = NULL) {
    stack_type x, y;
    StackErr possible_error = NO_ERROR;
    if (possible_error = StackPop(stk, &x, err)) {
        return possible_error;
    }
    if (possible_error = StackPop(stk, &y, err)) {
        return possible_error;
    }
    return StackPush(stk, x + y, err);
}

StackErr calculate_sub(struct Stack * stk, struct StackErrData * err = NULL) {
    stack_type x, y;
    StackErr possible_error = NO_ERROR;
    if (possible_error = StackPop(stk, &x, err)) {
        return possible_error;
    }
    if (possible_error = StackPop(stk, &y, err)) {
        return possible_error;
    }
    return StackPush(stk, x - y, err);
}

StackErr calculate_mul(struct Stack * stk, struct StackErrData * err = NULL) {
    stack_type x, y;
    StackErr possible_error = NO_ERROR;
    if (possible_error = StackPop(stk, &x, err)) {
        return possible_error;
    }
    if (possible_error = StackPop(stk, &y, err)) {
        return possible_error;
    }
    return StackPush(stk, x * y, err);
}

StackErr calculate_div(struct Stack * stk, struct StackErrData * err = NULL) {
    stack_type x, y;
    StackErr possible_error = NO_ERROR;
    if (possible_error = StackPop(stk, &x, err)) {
        return possible_error;
    }
    if (possible_error = StackPop(stk, &y, err)) {
        return possible_error;
    }
    if (y == 0) {
        printf("error x/0\n");
        return WAS_ERROR;
    }
    return StackPush(stk, x / y, err);
}

StackErr calculate_out(struct Stack * stk, struct StackErrData * err = NULL) {
    stack_type x;
    StackErr possible_error = NO_ERROR;
    if (possible_error = StackPop(stk, &x, err)) {
        return possible_error;
    }
    printf("%d\n", x);
    return NO_ERROR;
}