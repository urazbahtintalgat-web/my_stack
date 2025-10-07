//#include "asembler.h"

#include "calculate.h"
#include "work_with_file.h"

#include <stdio.h>

int check_calculate_comand(struct line * comands, size_t index, const char * comand, size_t comand_length) {
    if (memcmp(comands[index].begin, comand, comand_length) == 0) {
        return 1;
    }
    return 0;
}

#define CHECK_CALCULATE_COMAND(comands, index, comand) check_calculate_comand(comands, index, comand, sizeof(comand) - 1)

int main() {
    char human_file[] = "human_cmd.txt";
    size_t read = 0;
    char * whole_human_text = readfile(human_file, &read);
    if (whole_human_text == NULL) {
        printf("Human file was not opened\n");
        return 1;
    }

    int comand_amount = count_lines(whole_human_text);
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
    
    for (size_t i = 0; i < comand_amount; i++) {
        if (CHECK_CALCULATE_COMAND(comands, i, "PUSH")) {
            int x = atoi(comands[i].begin + sizeof("PUSH") - 1);
            unsigned char cmd = PUSH;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1 || fwrite(&x, sizeof(x), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "ADD")) {
            unsigned char cmd = ADD;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "SUB")) {
            unsigned char cmd = SUB;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "MUL")) {
            unsigned char cmd = MUL;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "DIV")) {
            unsigned char cmd = DIV;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "OUT")) {
            unsigned char cmd = OUT;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
        if (CHECK_CALCULATE_COMAND(comands, i, "HLT")) {
            unsigned char cmd = HLT;
            if (fwrite(&cmd, sizeof(cmd), 1, machine_text) != 1) {
                printf("invalid comand %d\n", i + 1);
                break;
            }
            continue;
        }
    }

    fclose(machine_text);
    free(whole_human_text);
    free(comands);

    return 0;
}