#ifndef ASEMBLER_H
#define ASEMBLER_H

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
    ASM_JMP  = 21,
    ASM_JB   = 22,
    ASM_JBE  = 23,
    ASM_JA   = 24,
    ASM_JAE  = 25,
    ASM_JE   = 26,
    ASM_JNE  = 27
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
    "ASM_POPR",//12
    "",//13
    "",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "ASM_JMP",//21
    "ASM_JB ",//22
    "ASM_JBE",//23
    "ASM_JA ",//24
    "ASM_JAE",//25
    "ASM_JE ",//26
    "ASM_JNE" //27
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

#endif //ASEMBLER_H