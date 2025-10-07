#include "processor.h"

#include "calculate.h"
#include "my_stack.h"
#include "work_with_file.h"

#include <stdio.h>

ProcessorErr ProcessorInit(struct ProcessorStruct * processor, size_t capacity, processor_error_storage_type * err) {
    /*
    asserts
    */
    struct Stack data = {};
    if (StackInit(&data, capacity)) {
        printf("Was error with stack");
        return PROCESSOR_WAS_ERROR;
    }
    processor->data = data;
    processor->code = 
}


int main() {
    
}