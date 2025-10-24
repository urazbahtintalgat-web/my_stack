#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "my_stack.h"
#include "asembler.h"
typedef unsigned int processor_error_storage_type;
#define REGISTERS_AMOUNT 16
#define ADRESSES_LENGTH 64
#define RAM_MEMORY_AMOUNT 100
/**
 * @brief Структура для хранения процессора
 * 
 * @param data            стек для хранения данный внутри процессора
 * @param addresses       стек для хранения адресов возврата в процессоре
 * @param code            массив с кодом который выполняет процессор
 * @param ram_count       размер оперативной памяти
 * @param ram             массив оперативной памяти
 * @param x               размер видео памяти по x
 * @param y               размер видео памяти по y
 * @param vram            массив видео памяти
 * @param program_counter счетчик нынешней команды
 * @param code_size       размер кода
 * @param registers       массив с регистрами
 */
struct ProcessorStruct {
    Stack data;
    Stack addresses;
    int * code;

    size_t ram_count;
    int * ram;

    size_t x;
    size_t y;
    char * vram;

    size_t program_counter;
    size_t code_size;
    int registers[REGISTERS_AMOUNT];
};

enum ProcessorErr {
    PROCESSOR_WAS_ERROR                =     -1,
    PROCESSOR_NO_ERROR                 =      0,
    //-----------------------------------
    PROCESSOR_NOT_INIT                 = 1 << 0,
    PROCESSOR_NULL_CODE                = 1 << 1,
    PROCESSOR_NEGATIVE_PROGRAM_COUNTER = 1 << 2,
    PROCESSOR_REGISTERS_OVER           = 1 << 3,
    CODE_FILE_NOT_OPENED               = 1 << 4,
    PROCESSOR_ALLOC_FAILED             = 1 << 5,
    PROCESSOR_INVALID_CODE_SIZE        = 1 << 6,
    PROCESSOR_PC_OUT_OF_CODE           = 1 << 7
};

//----------------------------------------------------------------

ProcessorErr ProcessorInit(struct ProcessorStruct * processor, size_t capacity, size_t ram_count, size_t x, size_t y, processor_error_storage_type * err);

//----------------------------------------------------------------

#endif//PROCESSOR_H