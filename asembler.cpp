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
    for (int reg = 0; reg < (sizeof(RegisterNames) / sizeof(RegisterNames[0])); reg++) {
        if (strncmp(now_register, RegisterNames[reg], 2) == 0) {
            printf("register %s", RegisterNames[reg]);////////////////////////////
            if (fwrite(&reg, sizeof(int), 1, machine_text) != 1) {
                return 0;
            }
            return 1;
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
    int ip = 0;
    for (int dobule_run = 0; dobule_run < 2; dobule_run++) {

        int ip = 0;
        printf("-----RUN %d-----\n", dobule_run + 1);


        for (size_t i = 0; i < comand_amount; i++) {
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
            }

            int now_comand_length = 0;
            while (now_comand_length < COMANDS_MAX_LENGTH && 
                   comands[i].begin[now_comand_length] != '\0' && 
                   comands[i].begin[now_comand_length] != ' ' && 
                   comands[i].begin[now_comand_length] != '\t' && 
                   comands[i].begin[now_comand_length] != '\n') {
                now_comand_length++;
            }
            if (now_comand_length == 0) {//если например пропуск строки
                continue;
            }
            printf("line %2d ", (int)i);////////////////////

            int command_found = 0;

            for (int cmd = 0; cmd < (sizeof(ComandNames) / sizeof(ComandNames[0])); cmd++) {
                if (ComandNames[cmd][0] == '\0') continue;

                if (strncmp(comands[i].begin, ComandNames[cmd] + 4, now_comand_length) == 0 && now_comand_length == strlen(ComandNames[cmd]) - 4) {
                    printf("(number %2d) comand %9s ", ip, ComandNames[cmd]);//////////////
                    command_found = 1;
                    if (dobule_run == 1 && fwrite(&cmd, sizeof(int), 1, machine_text) != 1) {
                        printf("assembler fweite error %s:%d\n", __FILE__, __LINE__);
                        return 1;
                    }
                    ip++;
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
                    if (ASM_JMP <= cmd && cmd <= ASM_JNE) {
                        const char * label_start = comands[i].begin + now_comand_length;

                        while (*label_start == ' ' || *label_start == '\t')
                        label_start++;
                        
                        if (dobule_run == 1) {
                            if (*label_start == ':') {
                                int label = atoi(label_start + 1);
                                fwrite(&labels[label], sizeof(int), 1, machine_text);
                                printf("(number %2d) jump -> :%d", ip, labels[label]);
                            } else {
                                int jump_ip = atoi(label_start);
                                fwrite(&jump_ip, sizeof(int), 1, machine_text);
                                printf("(number %2d) jump -> ip%d", ip, jump_ip);
                            }
                        }
                        ip++;
                    }
                    printf("\n");////////////////
                }
            }
            if (command_found == 0) {
                printf("not found comand\n");
                return 1;
            }
        }





















        /*
        if (CHECK_CALCULATE_COMAND(comands, i, "PUSH")) {
            int x = atoi(comands[i].begin + sizeof("PUSH") - 1);
            unsigned char cmd = PUSH;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1 || fwrite(&x, sizeof(x), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "ADD")) {
            unsigned char cmd = ADD;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "SUB")) {
            unsigned char cmd = SUB;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "MUL")) {
            unsigned char cmd = MUL;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "DIV")) {
            unsigned char cmd = DIV;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "OUT")) {
            unsigned char cmd = OUT;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "HLT")) {
            unsigned char cmd = HLT;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", (int)i + 1);
                break;
            }
            continue;
        }
        */
    }

    fclose(machine_text);
    free(whole_human_text);
    free(comands);

    return 0;
}