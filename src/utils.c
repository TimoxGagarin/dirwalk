#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <locale.h>
#include <sys/stat.h>
#include "headers/parameters_t.h"
#include "headers/item_t_funcs.h"

/**
 * @brief Получает параметры командной строки с использованием getopt.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Массив строк, представляющих аргументы командной строки.
 * @return Структура parameters_t, содержащая параметры программы.
 */
parameters_t get_options(int argc, char* argv[]){
    parameters_t params;
    params.path = (char*)malloc(255);  // Выделение памяти для пути по умолчанию
    params.symlinks = false;
    params.catalogs = false;
    params.files = false;
    params.sort = false;

    char ch;
    while((ch = getopt(argc, argv, "ldfs")) != EOF){
        switch (ch){
        case 'l':
            params.symlinks = true;
            break;
        case 'd':
            params.catalogs = true;
            break;
        case 'f':
            params.files = true;
            break;
        case 's':
            params.sort = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Если не указаны категории (каталоги, файлы, символические ссылки), включаем все по умолчанию.
    if(!(params.symlinks || params.catalogs || params.files)){
        params.symlinks = true;
        params.catalogs = true;
        params.files = true;
    }

    // Обновляем argc и argv, чтобы игнорировать аргументы, обработанные getopt.
    argc -= optind;
    argv += optind;

    // Если путь не указан, используем текущую директорию.
    if(argc == 0)
        strcpy(params.path, ".");
    else
        params.path = argv[0];

    return params;
}

/**
 * @brief Сравнивает две строки (используется для сортировки).
 *
 * @param a Первая строка для сравнения.
 * @param b Вторая строка для сравнения.
 * @return Результат сравнения строк.
 */
int compare_strings(const void *a, const void *b) {
    const item_t *item_l = (const item_t *)a;
    const item_t *item_r = (const item_t *)b;
    return strcoll((const char *)item_l->path, (const char *)item_r->path);
}

/**
 * @brief Сканирует файлы и каталоги в указанной директории и ее поддиректориях.
 *
 * @param params Параметры сканирования (фильтры и директория).
 * @param items Массив элементов item_t, который будет заполнен результатами сканирования.
 * @param count Количество элементов в массиве items.
 */
void scan_file(parameters_t params, item_t** items, int* count){
    DIR *directory;
    struct dirent *entry;
    struct stat fileStat;

    // Открываем папку.
    directory = opendir(params.path);

    // Проверка на успешное открытие папки.
    if (directory == NULL) {
        perror("Ошибка при открытии папки");
        exit(EXIT_FAILURE);
    }

    // Читаем содержимое папки.
    while ((entry = readdir(directory)) != NULL) {
        // Игнорируем ".." и "."
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
            continue;

        // Составляем полный путь к файлу.
        char* full_path = (char*)malloc(strlen(params.path) + strlen(entry->d_name) + 2);
        snprintf(full_path, strlen(params.path) + strlen(entry->d_name) + 2, "%s/%s", params.path, entry->d_name);

        // Получаем информацию о файле.
        if (stat(full_path, &fileStat) == -1) {
            perror("Ошибка при получении информации о файле");
            exit(EXIT_FAILURE);
        }

        // Если текущий элемент - директория, рекурсивно вызываем scan_file для нее.
        if(S_ISDIR(fileStat.st_mode)){
            parameters_t new_params;
            new_params.path = (char*)malloc(strlen(full_path) + 1);
            new_params.catalogs = params.catalogs;
            new_params.symlinks = params.symlinks;
            new_params.files = params.files;
            new_params.sort = params.sort;

            strcpy(new_params.path, full_path);
            scan_file(new_params, items, count);
        }
        
        // Выводим имена файлов в консоль, если соответствует условиям.
        if( (params.catalogs && S_ISDIR(fileStat.st_mode)) ||
            (params.files && S_ISREG(fileStat.st_mode)) ||
            (params.symlinks && S_ISLNK(fileStat.st_mode))){
            *items = (item_t*)realloc(*items, sizeof(item_t) * (++(*count)));
            (*items)[*count - 1] = create_item_t(full_path, fileStat);
        }
    }
    
    // Закрываем папку.
    closedir(directory);
}