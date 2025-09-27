#ifndef MY_STACK_H
#define MY_STACK_H
typedef int stack_type;
#define LEFT_CANARY  0xDEADBEEF
#define RIGHT_CANARY 0xFACEFEED


#define POISON 767676

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct Stack {
    stack_type * data;
    size_t size = 0;
    size_t capacity = 0;
    size_t real_capacity = 0;
};

enum StackErr {
    NO_ERROR              = 0,//нет ошибки
    WAS_ERROR             =-1,//показатель что хоть одна ошибка была
    //универсальные ошибки для проверки в каждой функции

    NULL_PTR              = 1,//передали нулевой указатель на стек
    NULL_DATA             = 2,//в стеке data нулевая
    INVALID_CAPACITY      = 3,//указана не положительная вместительность
    SIZE_BIGGER_CAPACITY  = 4,//размер больше вместительности
    REAL_CAPACITY_BAD     = 5,//real capacity != capacity + 2
    BREAK_LEFT_CANARY     = 6,//левая  граница испорчена
    BREAK_RIGHT_CANARY    = 7,//правая граница испорчена

    //частные ошибки в разных функциях

    ALLOC_FALED           = 8,//алокация памяти не выполнилась
    POP_EMPTY_STACK       = 9,//попытка достать элемент из пустого стека
    STACK_NOT_INIT        = 10,//стек не инициализирован

    ERROR_COUNT           = 11//количество вариаций ошибок
};

struct StackErrData {
    enum StackErr error;
    const char * file_name;
    const char * func_name;
    int line;
};

//------------------------------------------------------------------------------
StackErr StackOK(struct Stack * stk, struct StackErrData * err, const char file_name[], const char func_name[], const int line);

StackErr StackInit(struct Stack * stk, size_t capacity, struct StackErrData * err = NULL);

StackErr StackPush(struct Stack * stk, stack_type value, struct StackErrData * err = NULL);

StackErr StackPop(struct Stack * stk, stack_type * result, struct StackErrData * err = NULL);

void print_stack_error(struct StackErrData * err);

StackErr realloc_stack(struct Stack * stk, struct StackErrData * err, const char file_name[], const char func_name[], const int line);
//------------------------------------------------------------------------------

#define STACKOK(stk, err) StackOK(stk, err, __FILE__, __func__, __LINE__)
#define REALLOC_STACK(str, err) realloc_stack(stk, err, __FILE__, __func__, __LINE__)

//строчку (printf("Was error\n");\) можно удалить чтобы не засорять терминал уведомлениями об ошибках
#define STACK_ERROR_SAVE(err, err_type, File_name, Func_name, Line) \
    printf("Was error\n");\
    if (err != NULL) {\
        err[err_type].error = err_type;\
        err[err_type].file_name = File_name;\
        err[err_type].func_name = Func_name;\
        err[err_type].line = Line;\
        err[NO_ERROR].error = WAS_ERROR;\
    }/*else {\
        return WAS_ERROR;\
    }*/

// void Print(const char* place, ...);
// #define PRINT(...) Print(__FUNC__, ...)


void print_stack_error(struct StackErrData * err);

#endif //MY_STACK_H







/*
#define STACK_OK(err, stk)                                       \
    if (err != NULL) {                                           \
        if (stk == NULL) {                                       \
            STACK_ERROR_SAVE(err, NULL_PTR)                      \
            err[NO_ERROR].error = WAS_ERROR;                     \
            return NULL_PTR;                                     \
        } else {                                                 \
            if (stk->data == NULL) {                             \
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
*/