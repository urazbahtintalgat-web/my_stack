#include "work_with_file.h"

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>


/**
 * @brief Эта функция возвращает размер файла в байтах
 * 
 * @param file_name имя файла размер которого надо определить
 * 
 * @return размер файла в байтах либо -1 если открыть файл не вышло
 */
long int get_file_length(char* file_name) {
    struct stat text_stat;
    int n = stat(file_name, &text_stat);
    if (n != 0) {
        return -1;
    }
    return text_stat.st_size;
}

/**
 * @brief Эта функция считывает весь текстовый файл
 * 
 * @param file_name строка путь до твекстового файла
 * @param read указатель на число куда сохраняется количество считанных элементов
 * 
 * @return указатель на выделенную динамическую память в которую скопировался файл
 */
char* readfile(char* file_name, size_t *read) {
    assert(file_name);
    assert(read);

    long int size_text = get_file_length(file_name);
    if (size_text == -1) {
        return NULL;
    }

    char* massiv = (char*) calloc(size_text + 1, sizeof(char));
    if (massiv == NULL) {
        free(massiv);
        return NULL;
    }

    FILE* text = fopen(file_name, "r");
    if (text == NULL) {
        free(text);
        return NULL;
    }
    assert(text);

    *read = fread(massiv, sizeof(char), size_text, text);
    assert(read);
    massiv[*read] = '\0';
    return massiv;
}

/**
 * @brief эта функция считает количество строк в массиве где идут строки и между ними \н
 * 
 * @param line_massive указатель на длинную строку внутри которой много разных через \н
 * 
 * @return возвращает количество строк
 */
int count_lines(const char* line_massive) {
    assert(line_massive);

    char* ind = (char*) line_massive;
    int amount = 1;
    while (ind = strchr(ind, '\n')) {
        amount++;
        ind++;
    }
    return amount;
}

/**
 * @brief эта функция возвращает динамически выделенную память в которой содержиться массив
 *        структур для хранения указателя на начало и конец каждого слова
 * 
 * @param line_massive массив чаров который является множеством строк идущих подряд
 * @param line_amount количество строк
 * 
 * @return указатель на выделенный динамически массив из структур
 */
struct line* make_line_massive(char* line_massive, int line_amount) {
    assert(line_massive);
    assert(line_amount > 0);

    struct line * lines;
    lines = (struct line *) calloc(line_amount, sizeof(struct line));
    if (lines == NULL) {
        return NULL;
    }

    int n = 0;
    lines[0].begin = line_massive;

    char* ind = line_massive;
    char* saved_ind = ind;
    
    while (ind = strchr(ind, '\n')) {
        lines[n].end = ind - 1;
        n++;
        lines[n].begin = ind + 1;
        saved_ind = ind;
        ind++;
    }
    while (*saved_ind != '\0') {
        saved_ind++;
    }
    lines[n].end = saved_ind;
    assert(line_amount == n + 1);

    return lines;
}