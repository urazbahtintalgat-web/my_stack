#ifndef CALCULATE_H
#define CALCULATE_H
#include "my_stack.h"
#include <stdio.h>
#include <string.h>

enum CalculateCommands {
    PUSH = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    OUT = 5,
    HLT = 6
};

CalculateCommands choosecommand(char * command) {
    if (strcmp(command, "PUSH") == 0) {
        return PUSH;
    } else if (strcmp(command, "ADD") == 0) {
        return ADD;
    } else if (strcmp(command, "SUB") == 0) {
        return SUB;
    } else if (strcmp(command, "MUL") == 0) {
        return MUL;
    } else if (strcmp(command, "DIV") == 0) {
        return DIV;
    } else if (strcmp(command, "OUT") == 0) {
        return OUT;
    } else if (strcmp(command, "HLT") == 0) {
        return HLT;
    }

}

StackErr calculate(struct Stack * stk, struct StackErrData * err = NULL);

StackErr calculate_push(struct Stack * stk, int value, struct StackErrData * err = NULL);

StackErr calculate_add(struct Stack * stk, struct StackErrData * err = NULL);

StackErr calculate_sub(struct Stack * stk, struct StackErrData * err = NULL);

StackErr calculate_mul(struct Stack * stk, struct StackErrData * err = NULL);

StackErr calculate_div(struct Stack * stk, struct StackErrData * err = NULL);

StackErr calculate_out(struct Stack * stk, struct StackErrData * err = NULL);

#endif //CALCULATE_H