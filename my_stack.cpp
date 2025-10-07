#include "my_stack.h"
/**
 * @brief Эта функция проверяет стек на общие ошибки которые с ним могут произойти
 *        и записывает их в err если указатель на него не НУЛЛ
 * 
 * @return тк функция проверяет множество аспектов то она возвращает только то была ли хоть какая-то ошибка
 *         а если надо узнать все ошибки которые были то надо передать err и считать ошибки из него
 */
StackErr StackNotOK(struct Stack * stk, stack_error_storage_type * err, const char file_name[], const char func_name[], const int line) {
    StackErr flag = NO_ERROR;
    if (stk == NULL) {
        stack_error_save(err, NULL_PTR, file_name, line);
        printf("STACK_NULL_PTR ERROR %s:%d", file_name, line);
        flag = WAS_ERROR;
    } else {
        if (stk->data == NULL) {
            stack_error_save(err, NULL_DATA, file_name, line);
            flag = WAS_ERROR;
        } else {
            if (stk->data[0] != LEFT_CANARY) {
                stack_error_save(err, BREAK_LEFT_CANARY, file_name, line);
                flag = WAS_ERROR;
            }
            if (stk->data[stk->capacity + CANARY_SIZE] != RIGHT_CANARY) {
                stack_error_save(err, BREAK_RIGHT_CANARY, file_name, line);
                flag = WAS_ERROR;
            }
        }
        if (stk->size - CANARY_SIZE / 2 > stk->capacity) {              
            stack_error_save(err, SIZE_BIGGER_CAPACITY, file_name, line);
            flag = WAS_ERROR;
        }
    }
    return flag;
}

void set_canary(struct Stack * stk) {
    stk->data[0] = LEFT_CANARY;
    stk->data[stk->capacity + CANARY_SIZE - 1] = RIGHT_CANARY;
}

StackErr StackInit(struct Stack * stk, size_t capacity, stack_error_storage_type * err) {
    if (capacity <= 0) {
        stack_error_save(err, INVALID_CAPACITY, __FILE__, __LINE__);
        return INVALID_CAPACITY;
    }

    stk->data = (stack_type*) calloc(capacity + CANARY_SIZE, sizeof(stack_type));
    stk->size = 0 + CANARY_SIZE / 2;
    stk->capacity = capacity;

    if (stk->data == NULL) {
        stack_error_save(err, ALLOC_FALED, __FILE__, __LINE__);
        return ALLOC_FALED;
    }

    set_canary(stk);
    
    return STACKNOTOK(stk, err);
}


StackErr StackPush(struct Stack * stk, stack_type value, stack_error_storage_type * err) {
    if (STACKNOTOK(stk, err)) {
        return WAS_ERROR;
    }
    
    if (REALLOC_STACK(stk, err)) {
        return ALLOC_FALED;
    }

    stk->data[stk->size] = value;
    stk->size++;
    
    return STACKNOTOK(stk, err);
}


StackErr StackPop(struct Stack * stk, stack_type * result, stack_error_storage_type * err) {
    if (STACKNOTOK(stk, err)) {
        return WAS_ERROR;
    }

    if (stk->size == 1) {
        stack_error_save(err, POP_EMPTY_STACK, __FILE__, __LINE__);
        return POP_EMPTY_STACK;
    }
    --stk->size;
    *result = stk->data[stk->size];
    stk->data[stk->size] = (stack_type) POISON;

    return STACKNOTOK(stk, err);
}




StackErr realloc_stack(struct Stack * stk, stack_error_storage_type * err,  const char file_name[], const char func_name[], const int line) {
    if (stk->size < stk->capacity) {
        return NO_ERROR;
    }
    stack_type * buffer_data = stk->data;
    stk->capacity *= 2;
    stk->data = (stack_type*) realloc(stk->data, (stk->capacity + CANARY_SIZE) * sizeof(stack_type));
    if (stk->data == NULL) {
        stack_error_save(err, ALLOC_FALED, file_name, line);
        printf("stack was not expanded\n");
        stk->data = buffer_data;
        return ALLOC_FALED;
    }
    for (int i = stk->capacity / 2; i < stk->capacity; i++) {
        stk->data[i + CANARY_SIZE / 2] = POISON;
    }
    
    set_canary(stk);
    
    return NO_ERROR;
}

void print_stack_error(stack_error_storage_type err) {
    int flag_no_error = 1;
    for (int i = 0; i < sizeof(stack_error_storage_type); i++) {
        if (err | (1 << i) == 0) {
            printf("was error %d\n", i);
            flag_no_error = 0;
        }
    }
    if (flag_no_error) {
        printf("there is no errors\n");
    }
}

void stack_error_save(stack_error_storage_type * err, StackErr err_type, const char * file, const int line) {
    //printf(err_type\n)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Was error %s:%d\n", file, line);
    if (err != NULL) {
        *err |= err_type;
    }
}

size_t GetStackSize(struct Stack * stk) {
    return stk->size - CANARY_SIZE / 2;
}

size_t GetStackCapacity(struct Stack * stk) {
    return stk->capacity;
}