#ifndef ASEMBLER_H
#define ASEMBLER_H

/**
 * @brief Енам для хранения кодов команд для асемблера
 *        идейно 1-10 обычные команды
 *              11-20 связаные с регисторами
 *              21-40 связаные с метками и прыжками
 *              41-60 связаные с оперативной памятью
 */
enum AssemblerComands {
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

#define COMANDS_MAX_LENGTH 20
const char ComandNames[][COMANDS_MAX_LENGTH] = {
    "",//0
    "ASM_PUSH",//1
    "ASM_POP" ,//2
    "ASM_ADD" ,//3
    "ASM_SUB" ,//4
    "ASM_MUL" ,//5
    "ASM_DIV" ,//6
    "ASM_SQRT",//7
    "ASM_OUT" ,//8
    "ASM_HLT" ,//9
    "",//10
    "ASM_PUSHR",//11
    "ASM_POPR" ,//12
    "",//13
    "",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "",//21
    "ASM_JMP" ,//22
    "ASM_JB"  ,//23
    "ASM_JBE" ,//24
    "ASM_JA"  ,//25
    "ASM_JAE" ,//26
    "ASM_JE"  ,//27
    "ASM_JNE" ,//28
    "ASM_CALL",//29
    "ASM_RET" ,//30
    "",//31
    "",//32
    "",//33
    "",//34
    "",//35
    "",//36
    "",//37
    "",//38
    "",//39
    "",//40
    "ASM_PUSHM",//41
    "ASM_POPM"  //42
};

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
/*
struct asm_comand {
    int number;
    char name[COMANDS_MAX_LENGTH];
    int args;
};

asm_comand AsmComands[] = {
    {0, "", 0},
    {ASM_PUSH, "ASM_PUSH", }
};
*/

#endif //ASEMBLER_H