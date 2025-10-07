#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "my_stack.h"
typedef unsigned int processor_error_storage_type;

struct ProcessorStruct {
    Stack data;
    char* code;
    char* program_counter;
    int registers[16];
};

enum ProcessorErr {
    PROCESSOR_WAS_ERROR            =     -1,
    PROCESSOR_NO_ERROR             =      0,
    //-----------------------------------
    PROCESSOR_NOT_INIT             = 1 << 0,
    PROCESSOR_NULL_CODE            = 1 << 1,
    PROCESSOR_NULL_PROGRAM_COUNTER = 1 << 2,
    PROCESSOR_REGISTERS_OVER       = 1 << 3,
};

//----------------------------------------------------------------

ProcessorErr ProcessorInit(struct ProcessorStruct * processor, size_t capacity = 100, processor_error_storage_type * err = NULL);

//----------------------------------------------------------------

#endif//PROCESSOR_H