#include "processor_comands.h"

#include "processor.h"
#include "my_stack.h"

#include <math.h>
#include <stdio.h>

extern int RAM[];
int RAMX = 10;
int RAMY = 10;

ProcessorComand comand_list[] = {
    NULL,   //0
    DoPUSH, //1
    DoPOP,  //2
    DoADD,  //3
    DoSUB,  //4
    DoMUL,  //5
    DoDIV,  //6
    DoSQRT, //7
    DoOUT,  //8
    DoHLT,  //9
    NULL,   //10
    DoPUSHR,//11
    DoPOPR, //12
    NULL,   //13
    NULL,   //14
    NULL,   //15
    NULL,   //16
    NULL,   //17
    NULL,   //18
    NULL,   //19
    NULL,   //20
    NULL,   //21
    DoJMP,  //22
    DoJB ,  //23
    DoJBE,  //24
    DoJA ,  //25
    DoJAE,  //26
    DoJE ,  //27
    DoJNE,  //28
    DoCALL, //29
    DoRET,  //30
    NULL,   //31
    NULL,   //32
    NULL,   //33
    NULL,   //34
    NULL,   //35
    NULL,   //36
    NULL,   //37
    NULL,   //38
    NULL,   //39
    NULL,   //40
    DoPUSHM,//41
    DoPOPM, //42
    NULL,   //43
    NULL,   //44
    NULL,   //45
    NULL,   //46
    NULL,   //47
    NULL,   //48
    NULL,   //49
    NULL,   //50
    DoDROW, //51
};


//--------------------------
int OnePop(struct ProcessorStruct * processor, int * a, processor_error_storage_type * err) {
    return StackPop(&processor->data, a, NULL);
}
int TwoPop(struct ProcessorStruct * processor, int * a, int * b, processor_error_storage_type * err) {
    int error = 0;
    error += StackPop(&processor->data, a, NULL);
    error += StackPop(&processor->data, b, NULL);
    return error;
}

#define TWOPOP(processor, a, b) \
    if (StackPop(&processor->data, a, NULL)) {\
        return PROCESSOR_WAS_ERROR;\
    }\
    if (StackPop(&processor->data, b, NULL)) {\
        return PROCESSOR_WAS_ERROR;\
    }
//---------------------------


ProcessorErr DoPUSH(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int value = processor->code[processor->program_counter++];
    if (StackPush(&processor->data, value, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoPOP(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0;
    if (StackPop(&processor->data, &a, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoADD(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (StackPush(&processor->data, a + b, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoSUB(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (StackPush(&processor->data, b - a, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoMUL(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (StackPush(&processor->data, a * b, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoDIV(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)

    if (a == 0) {
        printf("Error: DIV zero(/0)\n");
        return PROCESSOR_WAS_ERROR;
    }
    if (StackPush(&processor->data, b / a, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoSQRT(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0;
    if (StackPop(&processor->data, &a, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    if (StackPush(&processor->data, (int) sqrt(a), NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoOUT(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0;
    if (StackPop(&processor->data, &a, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    printf("OUT %d\n", a);
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoHLT(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    
    printf("HLT program end\n");
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoPUSHR(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int reg = processor->code[processor->program_counter++];
    if (StackPush(&processor->data, processor->registers[reg], NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoPOPR(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int reg = processor->code[processor->program_counter++];
    int value = 0;
    if (StackPop(&processor->data, &value, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    processor->registers[reg] = value;
    return PROCESSOR_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////////////////////////

ProcessorErr DoJMP(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int index = processor->code[processor->program_counter++];
    processor->program_counter = index;
    return (index < processor->code_size) ? PROCESSOR_NO_ERROR : PROCESSOR_PC_OUT_OF_CODE;
}

ProcessorErr DoJB(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b < a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}

ProcessorErr DoJBE(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b <= a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}
ProcessorErr DoJA(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b > a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}
ProcessorErr DoJAE(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b >= a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}
ProcessorErr DoJE(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b == a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}
ProcessorErr DoJNE(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int a = 0, b = 0;
    TWOPOP(processor, &a, &b)
    if (b != a) {
        return DoJMP(processor, err);
    } else {
        processor->program_counter++;
        return PROCESSOR_NO_ERROR;
    }
}
ProcessorErr DoCALL(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int next_index = processor->code[processor->program_counter++];
    int last_index = processor->program_counter;
    if (StackPush(&processor->addresses, last_index, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    processor->program_counter = next_index;
    return (next_index < processor->code_size) ? PROCESSOR_NO_ERROR : PROCESSOR_PC_OUT_OF_CODE;
}
ProcessorErr DoRET(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int last_index = 0;
    if (StackPop(&processor->addresses, &last_index, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    processor->program_counter = last_index;
    return (last_index < processor->code_size) ? PROCESSOR_NO_ERROR : PROCESSOR_PC_OUT_OF_CODE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

ProcessorErr DoPUSHM(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int was_register = processor->code[processor->program_counter++];
    int index = 0;
    switch (was_register)
    {
    case 1: {
        int reg = processor->code[processor->program_counter++];
        index = processor->registers[reg];
        break;
    }
    
    case 0:
        index = processor->code[processor->program_counter++];
        break;
    
    default:
        printf("ERROR: was_register value = %d %s:%d\n", was_register, __FILE__, __LINE__);
        return PROCESSOR_WAS_ERROR;
        break;
    }
    if (StackPush(&processor->data, RAM[index], NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}
ProcessorErr DoPOPM(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    int was_register = processor->code[processor->program_counter++];
    int index = 0;
    switch (was_register)
    {
    case 1: {
        int reg = processor->code[processor->program_counter++];
        index = processor->registers[reg];
        break;
    }

    case 0:
        index = processor->code[processor->program_counter++];
        break;
    
    default:
        printf("ERROR: was_register value = %d %s:%d\n", was_register, __FILE__, __LINE__);
        return PROCESSOR_WAS_ERROR;
        break;
    }
    if (StackPop(&processor->data, RAM + index, NULL)) {
        return PROCESSOR_WAS_ERROR;
    }
    return PROCESSOR_NO_ERROR;
}

ProcessorErr DoDROW(struct ProcessorStruct * processor, processor_error_storage_type * err) {
    for (int y = 0; y < RAMY; y++) {
        for (int x = 0; x < RAMX; x++) {
            printf("%3d ", RAM[y * RAMX + x]);
        }
        printf("\n");
    }
    return PROCESSOR_NO_ERROR;
}






//int main() {
//    return 0;
//}