#ifndef MY_STACK_H
#define MY_STACK_H
typedef int stack_type;
#define POISON 767676
#define MAX_LENGTH 20

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct stack_t {
    stack_type * data;
    int size = 0;
    int capacity = 0;
};

enum StackErr_t {
    NO_ERROR              = 0,//нет ошибки
    WAS_ERROR             =-1,//показатель что хоть одна ошибка была
    //универсальные ошибки для проверки в каждой функции

    NULL_PTR              = 1,//передали нулевой указатель на стек
    NULL_DATA             = 2,//в стеке data нулевая
    INVALID_CAPACITY      = 3,//указана отрицательная вместительность
    INVALID_SIZE          = 4,//указана отрицательный размер
    SIZE_BIGGER_CAPACITY  = 5,//размер больше вместительности

    //частные ошибки в разных функциях

    ALLOC_FALED           = 6,//алокация памяти не выполнилась
    POP_EMPTY_STACK       = 7,//попытка достать элемент из пустого стека
    STACK_NOT_INIT        = 8,//стек не инициализирован

    ERROR_COUNT           = 9//количество вариаций ошибок
};

struct StackErr_t_data {
    enum StackErr_t error;
    char file_name[MAX_LENGTH];
    char function_name[MAX_LENGTH];
    int line;
};

//------------------------------------------------------------------------------
StackErr_t StackInit(stack_t * stk, int capacity, struct StackErr_t_data * err = NULL);

StackErr_t StackPush(stack_t * stk, stack_type value, struct StackErr_t_data * err = NULL);

stack_type StackPop(stack_t * stk, struct StackErr_t_data * err = NULL);
//------------------------------------------------------------------------------

#define STACK_ERROR_SAVE(err, err_type) \
    err[err_type].error = err_type;\
    strncpy(err[err_type].file_name, __FILE__, MAX_LENGTH);\
    err[err_type].file_name[MAX_LENGTH - 1] = '\0';\
    strncpy(err[err_type].function_name, __func__, MAX_LENGTH);\
    err[err_type].function_name[MAX_LENGTH - 1] = '\0';\
    err[err_type].line = __LINE__;\

#define STACK_OK(err, stk)                                       \
    if (err != NULL) {                                           \
        if (stk == NULL) {                                       \
            STACK_ERROR_SAVE(err, NULL_PTR)                      \
            err[NO_ERROR].error = WAS_ERROR;                     \
            return NULL_PTR;                                     \
        } else {                                                 \
            if (stk->data == NULL && __func__ != "StackInit") {  \
                STACK_ERROR_SAVE(err, NULL_DATA)                 \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
            if (stk->capacity < 0) {                             \
                STACK_ERROR_SAVE(err, INVALID_CAPACITY)          \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
            if (stk->size < 0) {                                 \
                STACK_ERROR_SAVE(err,INVALID_SIZE)               \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
            if (stk->size > stk->capacity) {                     \
                STACK_ERROR_SAVE(err,SIZE_BIGGER_CAPACITY)       \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
        }                                                        \
    }

void print_stack_error(struct StackErr_t_data * err);

#endif //MY_STACK_H