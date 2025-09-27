#include "my_stack.h"
/**
 * @brief Эта функция проверяет стек на общие ошибки которые с ним могут произойти
 *        и записывает их в err если указатель на него не НУЛЛ
 * 
 * @return тк функция проверяет множество аспектов то она возвращает только то была ли хоть какая-то ошибка
 *         а если надо узнать все ошибки которые были то надо передать err и считать ошибки из него
 */
StackErr StackOK(stack_t * stk, struct StackErrData * err, const char file_name[], const char func_name[], const int line) {
    StackErr flag = NO_ERROR;
    if (stk == NULL) {
        STACK_ERROR_SAVE(err, NULL_PTR, file_name, func_name, line)
        flag = WAS_ERROR;
    } else {
        if (stk->data == NULL) {
            STACK_ERROR_SAVE(err, NULL_DATA, file_name, func_name, line)
            flag = WAS_ERROR;
        } else {
            if (stk->data[0] != LEFT_CANARY) {
                STACK_ERROR_SAVE(err, BREAK_LEFT_CANARY, file_name, func_name, line)
                flag = WAS_ERROR;
            }
            if (stk->data[stk->real_capacity - 1] != RIGHT_CANARY) {
                STACK_ERROR_SAVE(err, BREAK_RIGHT_CANARY, file_name, func_name, line)
                flag = WAS_ERROR;
            }
        }
        if (stk->size > stk->capacity) {              
            STACK_ERROR_SAVE(err, SIZE_BIGGER_CAPACITY, file_name, func_name, line)
            flag = WAS_ERROR;
        }
        if (stk->capacity != stk->real_capacity - 2) {
            STACK_ERROR_SAVE(err, REAL_CAPACITY_BAD, file_name, func_name, line)
            flag = WAS_ERROR;
        }
    }
    return flag;
}


StackErr StackInit(stack_t * stk, size_t capacity, struct StackErrData * err) {
    if (capacity <= 0) {
        STACK_ERROR_SAVE(err, INVALID_CAPACITY, __FILE__, __func__, __LINE__)
        return INVALID_CAPACITY;
    }

    stk->data = (stack_type*) calloc(capacity + 2, sizeof(stack_type));
    stk->size = 1;
    stk->capacity = capacity;
    stk->real_capacity = capacity + 2;

    if (stk->data == NULL) {
        STACK_ERROR_SAVE(err, ALLOC_FALED, __FILE__, __func__, __LINE__)
        return ALLOC_FALED;
    }

    stk->data[0] = LEFT_CANARY;
    stk->data[stk->real_capacity - 1] = RIGHT_CANARY;
    
    return STACKOK(stk, err);
}


StackErr StackPush(stack_t * stk, stack_type value, struct StackErrData * err) {
    if (STACKOK(stk, err)) {
        return WAS_ERROR;
    }
    
    if (REALLOC_STACK(stk, err)) {
        return ALLOC_FALED;
    }

    stk->data[stk->size++] = value;
    
    return STACKOK(stk, err);
}


StackErr StackPop(stack_t * stk, stack_type * result, struct StackErrData * err) {

    if (STACKOK(stk, err)) {
        return WAS_ERROR;
    }

    if (stk->size == 1) {
        STACK_ERROR_SAVE(err, POP_EMPTY_STACK, __FILE__, __func__, __LINE__);
        return POP_EMPTY_STACK;
    }
    
    *result = stk->data[--stk->size];
    stk->data[stk->size] = (stack_type) POISON;

    return STACKOK(stk, err);
}

void print_stack_error(struct StackErrData * err) {
    if (err[0].error == NO_ERROR) {
        printf("There is no errors\n");
    } else {
        for (int i = 1; i < ERROR_COUNT; i++) {
            if ((int)err[i].error != (int) NO_ERROR){
                printf("ERROR %d: in file %s\nin function %s\n in line %d\n\n",
                        err[i].error, err[i].file_name, err[i].func_name, err[i].line);
            }
        }
    }
}


StackErr realloc_stack(stack_t * stk, struct StackErrData * err,  const char file_name[], const char func_name[], const int line) {
    if (stk->size >= stk->capacity) {
        stk->capacity *= 2;
        stk->data[stk->real_capacity - 1] = POISON;
        stk->real_capacity = stk->capacity + 2;
        stk->data = (stack_type*) realloc(stk->data, stk->real_capacity * sizeof(stack_type));
        if (stk->data == NULL) {
            STACK_ERROR_SAVE(err, ALLOC_FALED, file_name, func_name, line)
            return ALLOC_FALED;
        }
        stk->data[stk->real_capacity - 1] = RIGHT_CANARY;
    }
    return NO_ERROR;
}