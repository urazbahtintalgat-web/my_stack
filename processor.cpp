#include "processor.h"

#include "calculate.h"
#include "my_stack.h"
#include "work_with_file.h"
#include "asembler.h"
#include "processor_comands.h"

#include <stdio.h>

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
    /*
    asserts
    */
    struct Stack data = {};
    stack_error_storage_type stackerr = 0;
    if (StackInit(&data, capacity, &stackerr)) {
        printf("Was error with stack\n");
        return PROCESSOR_WAS_ERROR;
    }
    processor->data = data;
    processor->code = NULL;
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

    return (text_stat.st_size / 4);
}

ProcessorErr ReadCode(struct ProcessorStruct * processor, const char * file_name, processor_error_storage_type * err) {
    FILE * code_file = fopen(file_name, "rb");
    if (code_file == NULL) {
        printf("processor code file was not opened\n");
        if (err) *err |= CODE_FILE_NOT_OPENED;
        return CODE_FILE_NOT_OPENED;
    }

    //потом сделаю не константное
    
    processor->code = (int *) calloc(1000, sizeof(int));
    if (processor->code == NULL) {
        printf("processor allocated error\n");
        if (err) *err |= PROCESSOR_ALLOC_FAILED;
        fclose(code_file);
        return PROCESSOR_ALLOC_FAILED;
    }

    size_t bytes_read = fread(processor->code, sizeof(int), 1000, code_file);
    fclose(code_file);

    /*потом когда сделаю версию покурче не с константным значением памяти тут будет if bytes_read != file_size assert*/

    processor->program_counter = 0;
    processor->code_size = get_int_code_size(file_name);

    printf("success read, %zu\n", bytes_read);
    return PROCESSOR_NO_ERROR;
}

void ProcessorDestroy(struct ProcessorStruct * processor) {
    if (processor == NULL) {
        printf("warning try to destroy NULL processor\n");
        return;
    }

    StackDestroy(&processor->data);

    if (processor->code != NULL) {
        free(processor->code);
        processor->code = NULL;
    }

    processor->program_counter = 0;

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

    int * end = processor->code + processor->code_size;

    while (processor->program_counter < processor->code_size) {
        if ((verefy_result = ProcessorVerify(processor, err))) {
            printf("was error\n");
            return verefy_result;
        }
        int comand = *(processor->code + processor->program_counter++);

        ProcessorErr (*NowComand)(struct ProcessorStruct*, processor_error_storage_type*);
        NowComand = comand_list[comand];
        NowComand(processor, err);
        if (comand == ASM_HLT) {
            return PROCESSOR_NO_ERROR;
        }

        /*
        switch (comand) {
            case PUSH: {
                if (processor->program_counter + sizeof(stack_type) > end) {
                    printf("Error try to PUSH after end of file\n");
                    return PROCESSOR_WAS_ERROR;
                }
                int value = ReadIntFromCode(&processor->program_counter);
                if ((calculate_push(&processor->data, value, NULL))) {
                    printf("calculate push error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("push %d\n", value);
                break;
            }

            case ADD:
                if ((calculate_add(&processor->data))) {
                    printf("calculate add error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("add\n");
                break;
            
            case SUB:
                if ((calculate_sub(&processor->data))) {
                    printf("calculate sub error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("sub\n");
                break;
            
            case MUL:
                if ((calculate_mul(&processor->data))) {
                    printf("calculate mul error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("mul\n");
                break;

            case DIV:
                if ((calculate_div(&processor->data))) {
                    printf("calculate div error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("div\n");
                break;
            
            case OUT:
                if ((calculate_out(&processor->data))) {
                    printf("calculate out error\n");
                    return PROCESSOR_WAS_ERROR;
                }
                printf("out\n");
                break;

            case HLT:
                printf("hlt program end\n");
                return PROCESSOR_NO_ERROR;
            
            default:
                printf("unknown comand %d\n", comand);
                return PROCESSOR_WAS_ERROR;
        }
        */

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
