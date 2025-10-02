#ifndef MY_STACK_H
#define MY_STACK_H
typedef int stack_type;
typedef unsigned int error_storage_type;

#define CANARY_SIZE 2
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
};

enum StackErr {
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

//struct StackErrData {
//    enum StackErr error;
//    const char * file_name;
//    const char * func_name;
//    int line;
//};

//------------------------------------------------------------------------------
StackErr StackOK(struct Stack * stk, error_storage_type * err, const char file_name[], const char func_name[], const int line);

StackErr StackInit(struct Stack * stk, size_t capacity, error_storage_type * err = NULL);

StackErr StackPush(struct Stack * stk, stack_type value, error_storage_type * err = NULL);

StackErr StackPop(struct Stack * stk, stack_type * result, error_storage_type * err = NULL);

void print_stack_error(error_storage_type err);

StackErr realloc_stack(struct Stack * stk, error_storage_type * err, const char file_name[], const char func_name[], const int line);

void set_canary(struct Stack * stk);
//------------------------------------------------------------------------------

#define STACKOK(stk, err) StackOK(stk, err, __FILE__, __func__, __LINE__)
#define REALLOC_STACK(str, err) realloc_stack(stk, err, __FILE__, __func__, __LINE__)

//строчку (printf("Was error\n");\) можно удалить чтобы не засорять терминал уведомлениями об ошибках


void stack_error_save(error_storage_type * err, StackErr err_type, const char * file, const int line) {
    //printf(err_type\n)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Was error %s:%d\n", file, line);
    if (err != NULL) {
        *err |= err_type;
    }
}
/*#define STACK_ERROR_SAVE(err, err_type, File_name, Func_name, Line) \
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


void print_stack_error(error_storage_type * err);

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