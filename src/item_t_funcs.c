#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "headers/item_t.h"

/**
 * @brief Создает структуру item_t на основе переданного пути и информации о файле.
 *
 * @param path Путь к файлу или каталогу.
 * @param fileStat Информация о файле, полученная с использованием stat.
 * @return Созданная структура item_t.
 */
item_t create_item_t(char* path, struct stat fileStat){
    item_t ret;

    // Выделение памяти для хранения пути и копирование пути в структуру.
    ret.path = (char*)malloc(strlen(path) + 1);
    strcpy(ret.path, path);

    // Определение типа элемента в зависимости от режима файла.
    if(S_ISREG(fileStat.st_mode)) ret.type = file;
    if(S_ISLNK(fileStat.st_mode)) ret.type = symlink;
    if(S_ISDIR(fileStat.st_mode)) ret.type = dir;

    return ret;
}

/**
 * @brief Выводит элементы массива item_t на экран.
 *
 * @param items Массив элементов item_t.
 * @param count Количество элементов в массиве.
 */
void print_items(item_t* items, int count){
    for(int i = 0; i < count; i++)
        printf("%s\n", items[i].path);
}

/**
 * @brief Освобождает память, выделенную под элементы и их пути.
 *
 * @param items Массив элементов item_t.
 * @param count Количество элементов в массиве.
 */
void free_items(item_t* items, int count){
    for(int i = 0; i < count; i++)
        free(items[i].path);
    free(items);
}