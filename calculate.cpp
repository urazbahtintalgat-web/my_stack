#include "calculate.h"

#include "my_stack.h"
#include "work_with_file.h"

#include <stdio.h>
#include <string.h> 

#define COMMAND_MAX_LEN 5

/*
CalculateErr human_calculate(struct Stack * stk, calculate_error_storage_type * err) {
    while (1) {
        char command[COMMAND_MAX_LEN + 1] = "";
        scanf("%s", command);

        CalculateCommands x = choose_command(command);
        if (x == HLT) {
            return CALCULATE_NO_ERROR;
        }

        switch (x)
        {
        case PUSH: {
            int y = 0;
            scanf("%d", &y);
            return calculate_push(stk, y, err);
        }
            break;

        case ADD: {
            return calculate_add(stk, err);
        }
            break;

        case SUB: {
            return calculate_sub(stk, err);
        }
            break;

        case MUL: {
            return calculate_mul(stk, err);
        }
            break;

        case DIV: {
            return calculate_div(stk, err);
        }
            break;

        case OUT: {
            return calculate_out(stk, err);
        }
            break;
        
        default:
            break;
        }

    }
}
*/
CalculateErr pop_two_elements(struct Stack * stk, stack_type * x, stack_type * y, calculate_error_storage_type * err) {
    if (StackPop(stk, x, err)) {
        return CALCULATE_STACK_ERROR;
    }
    if (StackPop(stk, y, err)) {
        return CALCULATE_STACK_ERROR;
    }
    return CALCULATE_NO_ERROR;
}


CalculateErr calculate_push(struct Stack * stk, stack_type value, calculate_error_storage_type * err) {
    return (StackPush(stk, value, err)) ? CALCULATE_STACK_ERROR : CALCULATE_NO_ERROR;
}

CalculateErr calculate_add(struct Stack * stk, calculate_error_storage_type * err) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = CALCULATE_NO_ERROR;
    if ((possible_error = pop_two_elements(stk, &x, &y, err))) {
        return possible_error;
    }
    
    return (StackPush(stk, x + y, err)) ? CALCULATE_STACK_ERROR : CALCULATE_NO_ERROR;
}

CalculateErr calculate_sub(struct Stack * stk, calculate_error_storage_type * err) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = CALCULATE_NO_ERROR;
    if ((possible_error = pop_two_elements(stk, &x, &y, err))) {
        return possible_error;
    }
    
    return (StackPush(stk, x - y, err)) ? CALCULATE_STACK_ERROR : CALCULATE_NO_ERROR;
}

CalculateErr calculate_mul(struct Stack * stk, calculate_error_storage_type * err) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = CALCULATE_NO_ERROR;
    if ((possible_error = pop_two_elements(stk, &x, &y, err))) {
        return possible_error;
    }
    
    return (StackPush(stk, x * y, err)) ? CALCULATE_STACK_ERROR : CALCULATE_NO_ERROR;
}

CalculateErr calculate_div(struct Stack * stk, calculate_error_storage_type * err) {
    stack_type x = 0, y = 0;
    CalculateErr possible_error = CALCULATE_NO_ERROR;
    if ((possible_error = pop_two_elements(stk, &x, &y, err))) {
        return possible_error;
    }
    
    if (y == 0) {
        printf("error x / 0\n");
        return CALCULATE_WAS_ERROR;
    }
    return (StackPush(stk, x / y, err)) ? CALCULATE_STACK_ERROR : CALCULATE_NO_ERROR;
}

CalculateErr calculate_out(struct Stack * stk, calculate_error_storage_type * err) {
    stack_type x;
    if (StackPop(stk, &x, err)) {
        return CALCULATE_STACK_ERROR;
    }
    printf("%d\n", x);///зависит от stack_type
    return CALCULATE_NO_ERROR;
}
