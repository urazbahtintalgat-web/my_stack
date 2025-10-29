#include "asembler.h"

#include "calculate.h"
#include "work_with_file.h"

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

void assembler_init(AssemblerStruct * assembler, char * machine_text, line * comands, int comands_amount) {
    assert(machine_text);
    assert(assembler);
    assert(comands);

    assembler->machine_text = machine_text;//--------------
    assembler->comands = comands;
    assembler->comands_amount = comands_amount;
    //assembler->labels
    assembler->ip = 0;
    assembler->comands_ip = 0;
    assembler->machine_code = (int *) calloc(comands_amount * 5, sizeof(int));
    
    assert(assembler->machine_code);

    assembler->machine_index = 0;
    assembler->double_run = 0;
}

int run_preparing(AssemblerStruct * assembler, char * human_file, char * machine_file) {
    size_t read = 0;
    char * whole_human_text = readfile(human_file, &read);
    if (whole_human_text == NULL) {
        printf("Human file was not opened\n");
        return 1;
    }

    int comand_amount = count_lines(whole_human_text);
    printf("comands amount %d\n\n", comand_amount);
    struct line * comands = make_line_massive(whole_human_text, comand_amount);
    if (comands == NULL) {
        printf("memory was not allocated");
        free(whole_human_text);
        return 1;
    }
    printf("%s\n", comands->begin);////не обязательно
    /*
    FILE * machine_text = fopen(machine_file, "wb");
    if (machine_text == NULL) {
        printf("Cannot create machine code file\n");
        free(whole_human_text);
        free(comands);
        return 1;
    }
    */

    //НАЧАЛО ЗАПИСИ В СТРУКТУРУ

    assembler_init(assembler, machine_file, comands, comand_amount);
    return 0;
}

void assembler_destroy(AssemblerStruct * assembler) {
    if (assembler == NULL) {
        printf("Try to destroy NULL assembler struct\n");
        return;
    }
}

int check_calculate_comand(struct line * comands, size_t index, const char * comand, size_t comand_length) {
    if (memcmp(comands[index].begin, comand, comand_length) == 0) {
        return 1;
    }
    return 0;
}
#define CHECK_CALCULATE_COMAND(comands, index, comand) check_calculate_comand(comands, index, comand, sizeof(comand) - 1)

int fwrite_register(AssemblerStruct * assembler, const char * now_register) {
    for (int reg = 0; (unsigned long)reg < REGISTERS_AMOUNT; reg++) {
        if (strncmp(now_register, RegisterNames[reg], REGISTERS_LENGTH) == 0) {
            printf("register %s", RegisterNames[reg]);////////////////////////////
            assembler->machine_code[assembler->machine_index++] = reg;
            return 0;
        }
    }
    printf("ERROR: register was not found, %s:%d\n", __FILE__, __LINE__);
    return 1;
}

/**
 * @brief Функция которая проверяет метка ли идет на этой строке
 * 
 * @return Возврацает | 1 если это метка | 0 если это не метка | -1 при ошибке
 */
int check_label(AssemblerStruct * assembler, size_t i) {
    if (assembler->comands[i].begin[0] == ':') {
        int label = atoi(assembler->comands[i].begin + 1);
        if (label >= 0 && label < LABEL_AMOUNT) {
            assembler->labels[label] = assembler->ip;
            printf("line %2d label :%d -> ip%d\n", (int)i, label, assembler->labels[label]);
        } else {
            printf("\nlabel %d out of range (0-%d)\n", label, LABEL_AMOUNT - 1);
            return -1;
        }
        return 1;
    }
    return 0;
}

int write_comand(AssemblerStruct * assembler, int cmd, int * command_found) {
    printf("(number %2d) comand %9s ", assembler->ip, Assembler_Comands_massiv[cmd].name);
    *command_found = 1;
    if (assembler->double_run == 1) {
        assembler->machine_code[assembler->machine_index++] = cmd;
    }
    assembler->ip += 1;
    return 0;
}

int check_jumps(AssemblerStruct * assembler, size_t i, int cmd, int now_comand_length) {
    if (ASM_JMP <= cmd && cmd <= ASM_CALL) {
        const char * label_start = assembler->comands[i].begin + now_comand_length;

        while (*label_start == ' ' || *label_start == '\t')
            label_start++;

        if (assembler->double_run == 1) {
            if (*label_start == ':') {

                int label = atoi(label_start + 1);
                if (0 > label || label > LABEL_AMOUNT) return 1;
                assembler->machine_code[assembler->machine_index++] = assembler->labels[label];
                printf("(number %2d) jump -> :%d(ip%d)", assembler->ip, label, assembler->labels[label]);

            } else {

                int jump_ip = atoi(label_start);
                assembler->machine_code[assembler->machine_index++] = jump_ip;
                printf("(number %2d)jump -> ip%d", assembler->ip, jump_ip);

            }
        }
        assembler->ip += 1;
    }
    return 0;
}

int check_arguments(AssemblerStruct * assembler, size_t i, int cmd, int now_comand_length) {
    if (cmd == ASM_PUSH || cmd == ASM_CIRCLE) {

        int value = atoi(assembler->comands[i].begin + now_comand_length);
        printf("(number %2d) %d", assembler->ip , value);
        if (assembler->double_run == 1) {
            assembler->machine_code[assembler->machine_index++] = value;
        }

        assembler->ip += 1;
        return 0;

    }
    if (cmd == ASM_PUSHR || cmd == ASM_POPR) {

        printf("(number %2d) ", assembler->ip);
        if (assembler->double_run == 1) {
            if (fwrite_register(assembler, assembler->comands[i].begin + now_comand_length + 1)) {
                printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                return 1;
            }
        }
        assembler->ip += 1;
        return 0;

    }

    if (cmd == ASM_PUSHM || cmd == ASM_POPM) {

        printf("(number %2d) ", assembler->ip);
        const char * index_start = assembler->comands[i].begin + now_comand_length;

        while (*index_start == ' ' || *index_start == '\n')
            index_start++;
        if (*index_start++ != '[') {
            printf("ERROR: comand PUSHM/POPM continue without [\n");
            printf("ERROR: %s:%d\n", __FILE__, __LINE__);
            return 1;
        }
        
        int was_register = 0;
        if (isalpha(*index_start))
            was_register = 1;
        
        printf("was register = %d ", was_register);
        if (assembler->double_run == 1) {
            assembler->machine_code[assembler->machine_index++] = was_register;
        }
        
        assembler->ip += 1;

        switch (was_register) {
            case 1: {
                printf("(number %2d) ", assembler->ip);
                if (assembler->double_run == 1) {
                    if (fwrite_register(assembler, index_start)) {
                        printf("ERROR: assembler fwrite error with register value %s:%d\n", __FILE__, __LINE__);
                        return 1;
                    }
                }
                assembler->ip += 1;
                return 0;
            }

            case 0: {
                printf("(number %2d) ", assembler->ip);
                int value = atoi(index_start);
                printf("%d", value);
                if (assembler->double_run == 1) {
                    assembler->machine_code[assembler->machine_index++] = value;
                }
                assembler->ip += 1;
                return 0;
            }

            default:
                printf("ERROR: unknown error with pushm/popm %s:%d\n", __FILE__, __LINE__);
                return 1;
        }
    }

    if (cmd == ASM_PUSHV || cmd == ASM_POPV) {

        const char * index_start = assembler->comands[i].begin + now_comand_length;

        while (*index_start == ' ' || *index_start == '\n')
            index_start++;
        if (*index_start++ != '[') {
            printf("ERROR: comand PUSHV/POPV continue without [\n");
            printf("ERROR: %s:%d\n", __FILE__, __LINE__);
            return 1;
        }
        int y = atoi(index_start);
        while (isdigit(*index_start)) index_start++;
        if (*index_start != ']' || *(index_start + 1) != '[') {
            printf("ERROR: comand PUSHV/POPV continue without ][\n");
            printf("ERROR: %s:%d\n", __FILE__, __LINE__);
            return 1;
        }

        index_start += 2;

        int x = atoi(index_start);

        if (assembler->double_run == 1) {
            assembler->machine_code[assembler->machine_index++] = x;
            assembler->machine_code[assembler->machine_index++] = y;
        }

        printf("(number %2d) ", assembler->ip);
        printf("x=%d ", x);
        assembler->ip += 1;
        printf("(number %2d) ", assembler->ip);
        printf("y=%d ", y);
        assembler->ip += 1;
        return 0;
    }

    return 0;
}

int running_assembler(AssemblerStruct * assembler) {
    assert(assembler);

    assembler->ip = 0;
    printf("-----RUN %d-----\n", assembler->double_run + 1);

    for (size_t i = 0; i < assembler->comands_amount; i++) {
        //НАЧАЛО ПРОВЕРКИ НА МЕТКУ
        int label_res = 0;
        if ((label_res = check_label(assembler, i))) {
            if (label_res == -1) printf("Soft ERROR label was not detected don't miss that\n\n");
            continue;
        }
        //КОНЕЦ ПРОВЕРКИ НА МЕТКУ

        int now_comand_length = 0;
        // isspace isblank --------------
        while (now_comand_length < COMANDS_MAX_LENGTH && 
               assembler->comands[i].begin[now_comand_length] != '\0' && 
               assembler->comands[i].begin[now_comand_length] != ' '  && 
               assembler->comands[i].begin[now_comand_length] != '\t' && 
               assembler->comands[i].begin[now_comand_length] != '\n') {
            now_comand_length++;
        }
        if (now_comand_length == 0) continue;//ПРОПУСК СТРОКИ
        printf("line %2d ", (int)i);////////////////////

        int command_found = 0;//ФЛАГ ДЛЯ ПРОВЕРКИ НАШЛАСЬ ЛИ КОМАНДА

        for (int cmd = 0; cmd < COMMANDS_AMOUNT; cmd++) {
            //ПРОПУСТИТЬ ЕСЛИ ЭТО ПУСТАЯ КОМАНДА
            if (Assembler_Comands_massiv[cmd].name[0] == '\0') continue;
            //ПРОПУСТИТЬ ЕСЛИ КОМАНДА НЕ СОВПАЛА
            //printf("%d ", cmd);
            if (strncmp(assembler->comands[i].begin, Assembler_Comands_massiv[cmd].name + 4, now_comand_length) != 0 
                || now_comand_length != strlen(Assembler_Comands_massiv[cmd].name) - 4) continue;

            //ЗАПИСЬ КОМАНДЫ В ТЕКСТОВИК ДЛЯ ПРОЦЕССОРА
            if (write_comand(assembler, cmd, &command_found)) return 1;
            //КОНЕЦ ЗАПИСИ ДАЛЕЕ ИДУТ ПРОВЕРКИ НА АРГУМЕНТЫ


            //----------------------
            //ПРОВЕРКИ НА АРГУМЕНТЫ:
            //----------------------
            
            
            //ПРОВЕРКИ НА ПРЫЖКИ И ЗАПИСИ АЙПИШНИКОВ ПРИЖКОВ
            if (check_jumps(assembler, i, cmd, now_comand_length)) return 1;
            
            //КОНЕЦ ПРОВЕРКИ НА ПРЫЖКИ


            //НАЧАЛО ПРОВЕРКИ НА РАБОТУ С АРГУМЕНТАМИ ВНУТРИ ПРОЦЕССОРА
            if (check_arguments(assembler, i, cmd, now_comand_length)) return 1;
            
            //КОНЕЦ  ПРОВЕРКИ НА РАБОТУ С ОПЕРАТИСНОЙ ПОМЯТЬЮ


            printf("\n");
        }

        if (command_found == 0) {
            printf("ERROR: not found comand\n");
            return 1;
        }
    }
    return 0;
}

int make_machine_file(AssemblerStruct * assembler) {
    FILE * machine_text = fopen(assembler->machine_text, "wb");
    if (machine_text == NULL) {
        printf("Cannot create machine code file\n");
        return 1;
    }
    if (fwrite(assembler->machine_code, sizeof(int), assembler->machine_index, machine_text) != assembler->machine_index) {
        printf("ERROR: fwrite error in machine file\n");
        return 1;
    }
    printf("SUCESSFULL fwrite in machine file\n");
    fclose(machine_text);
    return 0;
}




int main() {
    int labels[LABEL_AMOUNT] = {0};
    AssemblerStruct assembler = {};
    AssemblerStruct * assembler_ptr = &assembler;
    char human_file[] = "human_cmd3.txt";
    char machine_file[] = "macine_cmd.txt";
    if (run_preparing(&assembler, human_file, machine_file)) return 1;


    assembler.double_run = 0;
    if (running_assembler(&assembler)) {
        return 1;
    }

    assembler.double_run = 1;
    if (running_assembler(&assembler)) {
        return 1;
    }

    for (int i = 0; i < assembler.machine_index; i++) {
        printf("%d ", assembler.machine_code[i]);
    }

    make_machine_file(&assembler);
    //fclose(machine_text);

    //free(whole_human_text);
    //free(comands);

    return 0;
}