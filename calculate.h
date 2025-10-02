#ifndef CALCULATE_H
#define CALCULATE_H
#include "my_stack.h"
#include <stdio.h>
#include <string.h>

typedef unsigned int calculate_error_storage_type;

enum CalculateCommands {
    PUSH = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    OUT = 5,
    HLT = 6
};

CalculateCommands choose_command(char * command) {
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

enum CalculateErr {
    NO_ERROR              =      0,//нет ошибки
    WAS_ERROR             =     -1,//показатель наличия ошибки
    //универсальные ошибки для проверки в каждой функции

    NULL_PTR              = 1 << 0,//передали нулевой указатель на стек
    NULL_DATA             = 1 << 1,//в стеке data нулевая
    INVALID_CAPACITY      = 1 << 2,//указана не положительная вместительность
    SIZE_BIGGER_CAPACITY  = 1 << 3,//размер больше вместительности
    BREAK_LEFT_CANARY     = 1 << 5,//левая  граница испорчена
    BREAK_RIGHT_CANARY    = 1 << 6,//правая граница испорчена

    //частные ошибки в разных функциях

    ALLOC_FALED           = 1 << 7,//алокация памяти не выполнилась
    POP_EMPTY_STACK       = 1 << 8,//попытка достать элемент из пустого стека
    STACK_NOT_INIT        = 1 << 9,//стек не инициализирован

};

CalculateErr calculate(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr calculate_push(struct Stack * stk, stack_type value, calculate_error_storage_type * err = NULL);

CalculateErr calculate_add(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr calculate_sub(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr calculate_mul(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr calculate_div(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr calculate_out(struct Stack * stk, calculate_error_storage_type * err = NULL);

CalculateErr pop_two_elements(struct Stack * stk, stack_type * x, stack_type * y, calculate_error_storage_type * err = NULL);

#endif //CALCULATE_H