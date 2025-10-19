#include "asembler.h"

#include "calculate.h"
#include "work_with_file.h"

#include <stdio.h>
#include <ctype.h>

#define LABEL_AMOUNT 10

int check_calculate_comand(struct line * comands, size_t index, const char * comand, size_t comand_length) {
    if (memcmp(comands[index].begin, comand, comand_length) == 0) {
        return 1;
    }
    return 0;
}
#define CHECK_CALCULATE_COMAND(comands, index, comand) check_calculate_comand(comands, index, comand, sizeof(comand) - 1)

int fwrite_register(const char * now_register, FILE * machine_text) {
    for (int reg = 0; (unsigned long)reg < (sizeof(RegisterNames) / sizeof(RegisterNames[0])); reg++) {
        if (strncmp(now_register, RegisterNames[reg], 2) == 0) {
            printf("register %s", RegisterNames[reg]);////////////////////////////
            if (fwrite(&reg, sizeof(int), 1, machine_text) != 1) {
                return 0;
            }
            return 1;
        }
    }
    printf("ERROR: register was not found, %s:%d\n", __FILE__, __LINE__);
    return 0;
}

/**
 * @brief Функция которая проверяет метка ли идет на этой строке
 * 
 * @return Возврацает | 1 если это метка | 0 если это не метка | -1 при ошибке
 */
int check_label(struct line * comands, int labels[], size_t i, int ip) {
    if (comands[i].begin[0] == ':') {
        int label = atoi(comands[i].begin + 1);
        if (label >= 0 && label < LABEL_AMOUNT) {
            labels[label] = ip;
            printf("line %2d label :%d -> ip%d\n", (int)i, label, labels[label]);
        } else {
            printf("\nlabel %d out of range (0-%d)\n", label, LABEL_AMOUNT - 1);
            return -1;
        }
        return 1;
    }
    return 0;
}

int write_comand(FILE * machine_text, int cmd, int * ip, int * comand_found, int double_run) {
    printf("(number %2d) comand %9s ", *ip, ComandNames[cmd]);
    *comand_found = 1;
    if (double_run == 1 && fwrite(&cmd, sizeof(int), 1, machine_text) != 1) {
        printf("assembler fwrite error %s:%d\n", __FILE__, __LINE__);
        return 1;
    }
    *ip += 1;
    return 0;
}

int check_jumps(FILE * machine_text, struct line * comands, int labels[], size_t i, int cmd, int now_comand_length, int * ip, int double_run) {
    if (ASM_JMP <= cmd && cmd <= ASM_CALL) {
        const char * label_start = comands[i].begin + now_comand_length;

        while (*label_start == ' ' || *label_start == '\t')
            label_start++;

        if (double_run == 1) {
            if (*label_start == ':') {
                int label = atoi(label_start + 1);
                if (fwrite(&labels[label], sizeof(int), 1, machine_text) != 1) {
                    printf("ERROR: jump ip fwrite error %s:%d\n", __FILE__, __LINE__);
                    return 1;
                }
                printf("(number %2d) jump -> :%d(ip%d)", *ip, label, labels[label]);
            } else {
                int jump_ip = atoi(label_start);
                if (fwrite(&jump_ip, sizeof(int), 1, machine_text) != 1) {
                    printf("ERROR: jump ip fwrite error %s:%d\n", __FILE__, __LINE__);
                    return 1;
                }
                printf("(number %2d)jump -> ip%d", *ip, jump_ip);
            }
        }
        *ip += 1;
    }
    return 0;
}

int check_arguments(FILE * machine_text, struct line * comands, int labels[], size_t i, int cmd, int now_comand_length, int * ip, int double_run) {
    if (cmd == ASM_PUSH) {
        int value = atoi(comands[i].begin + now_comand_length);
        printf("(number %2d) %d", *ip , value);
        if (double_run == 1 && fwrite(&value, sizeof(int), 1, machine_text) != 1) {
            printf("assembler fwrite error with push value %s:%d\n", __FILE__, __LINE__);
            return 1;
        }
        *ip += 1;
        return 0;
    }
    if (cmd == ASM_PUSHR || cmd == ASM_POPR) {
        printf("(number %2d) ", *ip);
        if (double_run == 1 && !fwrite_register(comands[i].begin + now_comand_length + 1, machine_text)) {
            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
            return 1;
        }
        *ip += 1;
        return 0;
    }

    if (cmd == ASM_PUSHM || cmd == ASM_POPM) {
        printf("(number %2d) ", *ip);
        const char * index_start = comands[i].begin + now_comand_length;

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
        if (double_run == 1 && fwrite(&was_register, sizeof(int), 1, machine_text) != 1) {
            printf("ERROR: assembler fwrite error with registr flag %s:%d\n", __FILE__, __LINE__);
            return 1;
        }
        *ip += 1;

        switch (was_register) {
            case 1: {
                printf("(number %2d) ", *ip);
                if (double_run == 1 && !fwrite_register(index_start, machine_text)) {
                    printf("ERROR: assembler fwrite error with register value %s:%d\n", __FILE__, __LINE__);
                    return 1;
                }
                *ip += 1;
                return 0;
                break;
            }

            case 0: {
                printf("(number %2d) ", *ip);
                int value = atoi(index_start);
                printf("%d", value);
                if (double_run == 1 && fwrite(&value, sizeof(int), 1, machine_text) != 1) {
                    printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                    return 1;
                }
                *ip += 1;
                return 0;
                break;
            }

            default:
                printf("ERROR: unknown error with pushm/popm %s:%d\n", __FILE__, __LINE__);
                return 1;
                break;
        }
    }

    return 0;
}




int main() {
    int labels[LABEL_AMOUNT] = {0};
    char human_file[] = "human_cmd.txt";
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
    printf("%s\n", comands->begin);

    FILE * machine_text = fopen("macine_cmd.txt", "wb");
    if (machine_text == NULL) {
        printf("Cannot create machine code file\n");
        free(whole_human_text);
        free(comands);
        return 1;
    }



    ///НАЧАЛО СКАНИРОВАНИЯ И ПЕРЕВОДА В КОД

    for (int dobule_run = 0; dobule_run < 2; dobule_run++) {

        int ip = 0;
        printf("-----RUN %d-----\n", dobule_run + 1);


        for (size_t i = 0; i < comand_amount; i++) {
            //НАЧАЛО ПРОВЕРКИ НА МЕТКУ
            int label_res = 0;
            if ((label_res = check_label(comands, labels, i, ip))) {
                if (label_res == -1) printf("Soft ERROR label was not detected don't miss that\n\n");
                continue;
            }
            //КОНЕЦ ПРОВЕРКИ НА МЕТКУ
            /*
            if (comands[i].begin[0] == ':') {//ПРОПИСАТЬ ЕСЛИ ЭТО МЕТКА
                int label = atoi(comands[i].begin + 1);
                if (label >= 0 && label < LABEL_AMOUNT) {
                    labels[label] = ip;
                    printf("line %2d label :%d -> ip%d\n", (int)i, label, labels[label]);
                } else {
                    printf("label %d out of range (0-%d)\n", label, LABEL_AMOUNT - 1);
                    return 1;
                }
                continue;
            }*/

            int now_comand_length = 0;
            while (now_comand_length < COMANDS_MAX_LENGTH && 
                   comands[i].begin[now_comand_length] != '\0' && 
                   comands[i].begin[now_comand_length] != ' '  && 
                   comands[i].begin[now_comand_length] != '\t' && 
                   comands[i].begin[now_comand_length] != '\n') {
                now_comand_length++;
            }
            if (now_comand_length == 0) continue;//ПРОПУСК СТРОКИ
            printf("line %2d ", (int)i);////////////////////

            int command_found = 0;//ФЛАГ ДЛЯ ПРОВЕРКИ НАШЛАСЬ ЛИ КОМАНДА

            for (int cmd = 0; cmd < (sizeof(ComandNames) / sizeof(ComandNames[0])); cmd++) {
                //ПРОПУСТИТЬ ЕСЛИ ЭТО ПУСТАЯ КОМАНДА
                if (ComandNames[cmd][0] == '\0') continue;
                //ПРОПУСТИТЬ ЕСЛИ КОМАНДА НЕ СОВПАЛА
                if (strncmp(comands[i].begin, ComandNames[cmd] + 4, now_comand_length) != 0 || now_comand_length != strlen(ComandNames[cmd]) - 4) continue;

                
                //ЗАПИСЬ КОМАНДЫ В ТЕКСТОВИК ДЛЯ ПРОЦЕССОРА
                if (write_comand(machine_text, cmd, &ip, &command_found, dobule_run)) return 1;
                //КОНЕЦ ЗАПИСИ ДАЛЕЕ ИДУТ ПРОВЕРКИ НА АРГУМЕНТЫ
                /*
                printf("(number %2d) comand %9s ", ip, ComandNames[cmd]);//////////////
                command_found = 1;
                if (dobule_run == 1 && fwrite(&cmd, sizeof(int), 1, machine_text) != 1) {
                    printf("assembler fweite error %s:%d\n", __FILE__, __LINE__);
                    return 1;
                }
                ip++;
                */


                //----------------------
                //ПРОВЕРКИ НА АРГУМЕНТЫ:
                //----------------------
                
                
                //ПРОВЕРКИ НА ПРЫЖКИ И ЗАПИСИ АЙПИШНИКОВ ПРИЖКОВ
                if (check_jumps(machine_text, comands, labels, i, cmd, now_comand_length, &ip, dobule_run)) return 1;
                
                //КОНЕЦ ПРОВЕРКИ НА ПРЫЖКИ
                /*
                if (ASM_JMP <= cmd && cmd <= ASM_CALL) {
                    const char * label_start = comands[i].begin + now_comand_length;

                    while (*label_start == ' ' || *label_start == '\t')
                        label_start++;
                    
                    if (dobule_run == 1) {
                        if (*label_start == ':') {
                            int label = atoi(label_start + 1);
                            fwrite(&labels[label], sizeof(int), 1, machine_text);
                            printf("(number %2d) jump -> :%d(ip%d)", ip, label, labels[label]);
                        } else {
                            int jump_ip = atoi(label_start);
                            fwrite(&jump_ip, sizeof(int), 1, machine_text);
                            printf("(number %2d) jump -> ip%d", ip, jump_ip);
                        }
                    }
                    ip++;
                }
                    */


                //НАЧАЛО ПРОВЕРКИ НА РАБОТУ С АРГУМЕНТАМИ ВНУТРИ ПРОЦЕССОРА
                if (check_arguments(machine_text, comands, labels, i, cmd, now_comand_length, &ip, dobule_run)) return 1;
                
                //КОНЕЦ  ПРОВЕРКИ НА РАБОТУ С ОПЕРАТИСНОЙ ПОМЯТЬЮ
                /*
                switch (cmd) {
                    case ASM_PUSH: {
                        int value = atoi(comands[i].begin + now_comand_length);
                        printf("(number %2d) %d",ip , value);////////////////////////
                        if (dobule_run == 1 && fwrite(&value, sizeof(int), 1, machine_text) != 1) {
                            printf("assembler fwrite error with push value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }
                    case ASM_PUSHR: {
                        printf("(number %2d) ", ip);
                        if (dobule_run == 1 && !fwrite_register(comands[i].begin + now_comand_length + 1, machine_text)) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }
                    case ASM_POPR: {
                        printf("(number %2d) ", ip);
                        if (dobule_run == 1 && !fwrite_register(comands[i].begin + now_comand_length + 1, machine_text)) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }
                    default:
                        break;
                }

                //КОНЕЦ  ПРОВЕРКИ НА РАБОТУ С АРГУМЕНТАМИ ВНУТРИ ПРОЦЕССОРА


                //НАЧАЛО ПРОВЕРКИ НА РАБОТУ С ОПЕРАТИВНОЙ ПАМЯТЬЮ
                
                switch (cmd)
                {
                case ASM_PUSHM: {
                    printf("(number %2d) ", ip);
                    const char * index_start = comands[i].begin + now_comand_length;

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
                    //ЗАПИСЫВАЕМ БУДЕТ ЛИ ДАЛЕЕ РЕГИСТР ИЛИ ПРОСТО ИНДЕКС
                    printf("was register = %d ", was_register);
                    if (dobule_run == 1 && !fwrite(&was_register, sizeof(int), 1, machine_text)) {
                        printf("ERROR: assembler fwrite error with registr flag %s:%d\n", __FILE__, __LINE__);
                        return 1;
                    }

                    ip++;
                    //ЗАПИСАЛИ   БУДЕТ ЛИ ДАЛЕЕ РЕГИСТР ИЛИ ПРОСТО ИНДЕКС

                    switch (was_register)
                    {
                    case 1: {
                        printf("(number %2d) ", ip);
                        if (dobule_run == 1 && !fwrite_register(index_start, machine_text)) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }
                    
                    case 0: {
                        printf("(number %2d) ", ip);
                        int value = atoi(index_start);
                        printf("%d", value);
                        if (dobule_run == 1 && fwrite(&value, sizeof(int), 1, machine_text) != 1) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }

                    default: {
                        printf("ERROR: unknown thing in pushm %s:%d\n", __FILE__, __LINE__);
                        return 1;
                        break;
                    }
                    }
                    break;
                }

                case ASM_POPM: {
                    printf("(number %2d) ", ip);
                    const char * index_start = comands[i].begin + now_comand_length;

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

                    //ЗАПИСЫВАЕМ БУДЕТ ЛИ ДАЛЕЕ РЕГИСТР ИЛИ ПРОСТО ИНДЕКС
                    printf("was register = %d ", was_register);
                    if (dobule_run == 1 && !fwrite(&was_register, sizeof(int), 1, machine_text)) {
                        printf("ERROR: assembler fwrite error with registr flag %s:%d\n", __FILE__, __LINE__);
                        return 1;
                    }

                    ip++;
                    //ЗАПИСАЛИ   БУДЕТ ЛИ ДАЛЕЕ РЕГИСТР ИЛИ ПРОСТО ИНДЕКС

                    switch (was_register)
                    {
                    case 1: {
                        printf("(number %2d) ", ip);
                        if (dobule_run == 1 && !fwrite_register(index_start, machine_text)) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }
                    
                    case 0: {
                        printf("(number %2d) ", ip);
                        int value = atoi(index_start);
                        printf("%d", value);
                        if (dobule_run == 1 && fwrite(&value, sizeof(int), 1, machine_text) != 1) {
                            printf("assembler fwrite error with pushr value %s:%d\n", __FILE__, __LINE__);
                            return 1;
                        }
                        ip++;
                        break;
                    }

                    default: {
                        printf("ERROR: unknown thing in pushm %s:%d\n", __FILE__, __LINE__);
                        return 1;
                        break;
                    }
                    }
                    break;
                }
                
                default:
                    break;
                }
                */
                


                printf("\n");////////////////
            }
            
            if (command_found == 0) {
                printf("not found comand\n");
                return 1;
            }
        }
    }

    fclose(machine_text);
    free(whole_human_text);
    free(comands);

    return 0;
}