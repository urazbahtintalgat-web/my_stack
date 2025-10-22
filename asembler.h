#ifndef ASEMBLER_H
#define ASEMBLER_H

#include <stdio.h>


#define LABEL_AMOUNT 10
#define COMANDS_MAX_LENGTH 20
/**
 * @brief Енам для хранения кодов команд для асемблера
 *        идейно 1-10 обычные команды
 *              11-20 связаные с регисторами
 *              21-40 связаные с метками и прыжками
 *              41-60 связаные с оперативной памятью
 */
enum AssemblerComandsEnum {
    ASM_PUSH = 1,
    ASM_POP  = 2,
    ASM_ADD  = 3,
    ASM_SUB  = 4,
    ASM_MUL  = 5,
    ASM_DIV  = 6,
    ASM_SQRT = 7,
    ASM_OUT  = 8,
    ASM_HLT  = 9,
    //10
    ASM_PUSHR= 11,
    ASM_POPR = 12,
    //13
    //14
    //15
    //16
    //17
    //18
    //19
    //20
    CF_INSTRS_START = 21,
    ASM_JMP  = 22,
    ASM_JB   = 23,
    ASM_JBE  = 24,
    ASM_JA   = 25,
    ASM_JAE  = 26,
    ASM_JE   = 27,
    ASM_JNE  = 28,
    ASM_CALL = 29,
    ASM_RET  = 30,
    CF_INSTRS_END = 31,
    //32
    //33
    //34
    //35
    //36
    //37
    //38
    //39
    //40
    ASM_PUSHM = 41,
    ASM_POPM  = 42
};


struct AssemblerComand {
    char name[COMANDS_MAX_LENGTH];
    int comand_ip;
    int args_amount;
    int opcode;
};

const struct AssemblerComand Assembler_Comands_massiv[] = {
    {"", 0, 0, 0},                           // 0
    {"ASM_PUSH", ASM_PUSH, 1, 1},            // 1
    {"ASM_POP" , ASM_POP , 1, 2},            // 2
    {"ASM_ADD" , ASM_ADD , 0, 3},            // 3
    {"ASM_SUB" , ASM_SUB , 0, 4},            // 4
    {"ASM_MUL" , ASM_MUL , 0, 5},            // 5
    {"ASM_DIV" , ASM_DIV , 0, 6},            // 6
    {"ASM_SQRT", ASM_SQRT, 0, 7},            // 7
    {"ASM_OUT" , ASM_OUT , 0, 8},            // 8
    {"ASM_HLT" , ASM_HLT , 0, 9},            // 9
    {"", 0, 0, 0},                           // 10
    {"ASM_PUSHR", ASM_PUSHR, 1, 11},         // 11
    {"ASM_POPR", ASM_POPR  , 1, 12},         // 12
    {"", 0, 0, 0},                           // 13
    {"", 0, 0, 0},                           // 14
    {"", 0, 0, 0},                           // 15
    {"", 0, 0, 0},                           // 16
    {"", 0, 0, 0},                           // 17
    {"", 0, 0, 0},                           // 18
    {"", 0, 0, 0},                           // 19
    {"", 0, 0, 0},                           // 20
    {"", 0, 0, 0},                           // 21
    {"ASM_JMP" , ASM_JMP , 1, 22},           // 22
    {"ASM_JB"  , ASM_JB  , 1, 23},           // 23
    {"ASM_JBE" , ASM_JBE , 1, 24},           // 24
    {"ASM_JA"  , ASM_JA  , 1, 25},           // 25
    {"ASM_JAE" , ASM_JAE , 1, 26},           // 26
    {"ASM_JE"  , ASM_JE  , 1, 27},           // 27
    {"ASM_JNE" , ASM_JNE , 1, 28},           // 28
    {"ASM_CALL", ASM_CALL, 1, 29},           // 29
    {"ASM_RET" , ASM_RET , 0, 30},           // 30
    {"", 0, 0, 0},                           // 31
    {"", 0, 0, 0},                           // 32
    {"", 0, 0, 0},                           // 33
    {"", 0, 0, 0},                           // 34
    {"", 0, 0, 0},                           // 35
    {"", 0, 0, 0},                           // 36
    {"", 0, 0, 0},                           // 37
    {"", 0, 0, 0},                           // 38
    {"", 0, 0, 0},                           // 39
    {"", 0, 0, 0},                           // 40
    {"ASM_PUSHM", ASM_PUSHM, 2, 41},         // 41
    {"ASM_POPM" , ASM_POPM , 2, 42}          // 42
};
#define COMMANDS_AMOUNT (sizeof(Assembler_Comands_massiv) / sizeof(Assembler_Comands_massiv[0]))



enum Registers {
    RA = 0,  // 0
    RB,      // 1
    RC,      // 2
    RD,      // 3
    RE,      // 4
    RF,      // 5
    RG,      // 6
    RH,      // 7
    RI,      // 8
    RJ,      // 9
    RK,      // 10
    RL,      // 11
    RM,      // 12
    RN,      // 13
    RO,      // 14
    RP       // 15
};

const char RegisterNames[][3] = {
    "RA",  // 0
    "RB",  // 1
    "RC",  // 2
    "RD",  // 3
    "RE",  // 4
    "RF",  // 5
    "RG",  // 6
    "RH",  // 7
    "RI",  // 8
    "RJ",  // 9
    "RK",  // 10
    "RL",  // 11
    "RM",  // 12
    "RN",  // 13
    "RO",  // 14
    "RP"   // 15
};

struct AssemblerStruct {
    FILE * machine_text;
    struct line * comands;
    int comands_amount;
    int labels[LABEL_AMOUNT] = {0};
    int ip;
    int comands_ip;
    int * machine_code;
    int machine_index;
    int double_run;
};


#endif //ASEMBLER_H