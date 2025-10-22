#ifndef PROCESSOR_COMANDS_H
#define PROCESSOR_COMANDS_H
#include "processor.h"
#include "my_stack.h"

typedef ProcessorErr (*ProcessorComand)(struct ProcessorStruct*, processor_error_storage_type*);
//------------------------------------------
ProcessorErr DoPUSH (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoPOP  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoADD  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoSUB  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoMUL  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoDIV  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoSQRT (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoOUT  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoHLT  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoPUSHR(struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoPOPR (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJMP  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJB   (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJBE  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJA   (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJAE  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJE   (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoJNE  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoCALL (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoRET  (struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoPUSHM(struct ProcessorStruct * processor, processor_error_storage_type * err);
ProcessorErr DoPOPM (struct ProcessorStruct * processor, processor_error_storage_type * err);

//------------------------------------------
int OnePop(struct ProcessorStruct * processor, int * a, processor_error_storage_type * err = NULL);
int TwoPop(struct ProcessorStruct * processor, int * a, int * b, processor_error_storage_type * err = NULL);

//------------------------------------------

// ProcessorComand GetCommand(command_id)
extern ProcessorComand comand_list[];



#endif //PROCESSOR_COMANDS_H