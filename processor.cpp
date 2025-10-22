#include "processor.h"

#include "calculate.h"
#include "my_stack.h"
#include "work_with_file.h"
#include "asembler.h"
#include "processor_comands.h"

#include <stdio.h>

int RAM[RAM_MEMORY_AMOUNT] = {};

ProcessorErr ProcessorVerify(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    ProcessorErr flag = PROCESSOR_NO_ERROR;
    if (processor == NULL) {
        if (err) *err |= PROCESSOR_NOT_INIT;
        return PROCESSOR_NOT_INIT;
    }

    if (StackNotOK(&processor->data, NULL, __FILE__, __func__, __LINE__)) {
        printf("Stack verefication fail\n");
        flag = PROCESSOR_WAS_ERROR;
    }

    if (StackNotOK(&processor->addresses, NULL, __FILE__, __func__, __LINE__)) {
        printf("Addresses stack verification fail\n");
        flag = PROCESSOR_WAS_ERROR;
    }

    if (processor->code == NULL) {
        if (err) *err |= PROCESSOR_NULL_CODE;
        flag = PROCESSOR_WAS_ERROR;
    }

    if (processor->program_counter < 0) {
        if (err) *err |= PROCESSOR_NEGATIVE_PROGRAM_COUNTER;
        flag = PROCESSOR_WAS_ERROR;
    }

    if (processor->code_size < 0) {
        if (err) *err |= PROCESSOR_INVALID_CODE_SIZE;
        flag = PROCESSOR_WAS_ERROR;
    }
    
    if (processor->program_counter >= processor->code_size) {
        if (err) *err |= PROCESSOR_PC_OUT_OF_CODE;
        flag = PROCESSOR_WAS_ERROR;
    }

    return flag;
}

ProcessorErr ProcessorInit(struct ProcessorStruct * processor, size_t capacity, processor_error_storage_type * err) {
    if (processor == NULL) {
        printf("Error: NULL processor pointer\n");
        return PROCESSOR_WAS_ERROR;
    }
    if (capacity <= 0) {
        printf("Error: invalid capacity <= 0\n");
        return PROCESSOR_WAS_ERROR;
    }
    
    struct Stack data = {};
    stack_error_storage_type stackerr = 0;
    if (StackInit(&data, capacity, &stackerr)) {
        printf("Was error with data stack\n");
        return PROCESSOR_WAS_ERROR;
    }
    struct Stack addresses = {};
    if (StackInit(&addresses, ADRESSES_LENGTH, &stackerr)) {
        printf("Was error with addresses stack\n");
        return PROCESSOR_WAS_ERROR;
    }
    processor->data = data;
    processor->addresses = addresses;
    processor->code = NULL;
    processor->ram = RAM;
    processor->program_counter = 0;
    processor->code_size = 0;
    for (size_t i = 0; i < REGISTERS_AMOUNT; i++) {
        processor->registers[i] = 0;
    }

    return PROCESSOR_NO_ERROR;
}

long int get_int_code_size(const char* file_name) {
    struct stat text_stat;
    int n = stat(file_name, &text_stat);
    if (n != 0) {
        return -1;
    }
    
    // ssize_t

    return (text_stat.st_size / sizeof(int));
}

ProcessorErr ReadCode(struct ProcessorStruct * processor, const char * file_name, processor_error_storage_type * err) {
    FILE * code_file = fopen(file_name, "rb");
    if (code_file == NULL) {
        printf("processor code file was not opened\n");
        if (err) *err |= CODE_FILE_NOT_OPENED;
        return CODE_FILE_NOT_OPENED;
    }

    processor->code_size = get_int_code_size(file_name);

    processor->code = (int *) calloc(processor->code_size, sizeof(int));
    if (processor->code == NULL) {
        printf("processor allocated error\n");
        if (err) *err |= PROCESSOR_ALLOC_FAILED;
        fclose(code_file);
        return PROCESSOR_ALLOC_FAILED;
    }

    size_t bytes_read = fread(processor->code, sizeof(int), processor->code_size, code_file);
    fclose(code_file);

    /*потом когда сделаю версию покурче не с константным значением памяти тут будет if bytes_read != file_size assert*/

    processor->program_counter = 0;

    printf("success read, %zu\n", bytes_read);
    return PROCESSOR_NO_ERROR;
}

void ProcessorDestroy(struct ProcessorStruct * processor) {
    if (processor == NULL) {
        printf("warning try to destroy NULL processor\n");
        return;
    }

    StackDestroy(&processor->data);
    StackDestroy(&processor->addresses);

    if (processor->code != NULL) {
        free(processor->code);
        processor->code = NULL;
    }

    processor->program_counter = 0;

    for (int i = 0; i < RAM_MEMORY_AMOUNT; i++) {
        processor->ram[i] = 0;
    }
    for (int i = 0; i < REGISTERS_AMOUNT; i++) {
        processor->registers[i] = 0;
    }
    printf("processor destroyed\n");
}

int ReadIntFromCode(unsigned char ** program_counter) {
    int value = *(int *)(*program_counter);
    *program_counter += sizeof(int);
    return value;
}

ProcessorErr WholeProgram(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    ProcessorErr verefy_result = PROCESSOR_NO_ERROR;
    if ((verefy_result = ProcessorVerify(processor, err))) {
        printf("was error\n");
        return verefy_result;
    }

    while (processor->program_counter < processor->code_size) {
        if ((verefy_result = ProcessorVerify(processor, err))) {
            printf("was error with running program: %s:%d\n", __FILE__, __LINE__);
            return verefy_result;
        }
        
        int comand = *(processor->code + processor->program_counter++);
        
        ProcessorErr (*NowComand)(struct ProcessorStruct*, processor_error_storage_type*);
        NowComand = comand_list[comand];
        
        if (NowComand == NULL) {
            printf("ERROR: NULL command for code %d\n", comand);
            return PROCESSOR_WAS_ERROR;
        }
        
        if (NowComand(processor, err)) {
            printf("error in %s:%d", __FILE__, __LINE__);
            return PROCESSOR_WAS_ERROR;
        }
        if (comand == ASM_HLT) {
            return PROCESSOR_NO_ERROR;
        }
    }
    return ProcessorVerify(processor, err);
}





int main() {
    processor_error_storage_type err = 0;
    struct ProcessorStruct processor;
    const char file_name[] = "macine_cmd.txt";

    if ((ProcessorInit(&processor, 1000, &err))) {
        printf("processor init error\n");
        return 1;
    }

    if ((ReadCode(&processor, file_name, &err))) {
        printf("read code error\n");
        return 1;
    }

    if ((WholeProgram(&processor, &err))) {
        printf("whole program error\n");
        return 1;
    }

    ProcessorDestroy(&processor);
    return 0;
}
