#ifndef MY_STACK_H
#define MY_STACK_H
typedef int stack_type;
#define POISON 767676

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct stack_t {
    stack_type * data;
    size_t size = 0;
    size_t capacity = 0;
};

enum StackErr {
    NO_ERROR              = 0,//нет ошибки
    WAS_ERROR             =-1,//показатель что хоть одна ошибка была
    //универсальные ошибки для проверки в каждой функции

    NULL_PTR              = 1,//передали нулевой указатель на стек
    NULL_DATA             = 2,//в стеке data нулевая
    INVALID_CAPACITY      = 3,//указана не положительная вместительность
    SIZE_BIGGER_CAPACITY  = 4,//размер больше вместительности

    //частные ошибки в разных функциях

    ALLOC_FALED           = 5,//алокация памяти не выполнилась
    POP_EMPTY_STACK       = 6,//попытка достать элемент из пустого стека
    STACK_NOT_INIT        = 7,//стек не инициализирован

    ERROR_COUNT           = 8//количество вариаций ошибок
};

struct StackErrData {
    enum StackErr error;
    const char * file_name;
    const char * func_name;
    int line;
};

//------------------------------------------------------------------------------
StackErr StackInit(stack_t * stk, size_t capacity, struct StackErrData * err = NULL);

StackErr StackPush(stack_t * stk, stack_type value, struct StackErrData * err = NULL);

stack_type StackPop(stack_t * stk, struct StackErrData * err = NULL);
//------------------------------------------------------------------------------

#define STACK_ERROR_SAVE(err, err_type, File_name, Func_name, Line) \
    if (err != NULL) {\
        err[err_type].error = err_type;\
        err[err_type].file_name = File_name;\
        err[err_type].func_name = Func_name;\
        err[err_type].line = Line;\
    }/*else {\
        return WAS_ERROR;\
    }*/

// void Print(const char* place, ...);
// #define PRINT(...) Print(__FUNC__, ...)

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
            if (stk->capacity <= 0) {                            \
                STACK_ERROR_SAVE(err, INVALID_CAPACITY)          \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
            if (stk->size > stk->capacity) {                     \
                STACK_ERROR_SAVE(err,SIZE_BIGGER_CAPACITY)       \
                err[NO_ERROR].error = WAS_ERROR;                 \
            }                                                    \
        }                                                        \
    }

void print_stack_error(struct StackErrData * err);

#endif //MY_STACK_H