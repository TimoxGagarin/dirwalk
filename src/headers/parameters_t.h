/**
 * @file parameters_t.h
 * @brief Определение структуры parameters_t для хранения параметров программы.
 */

#pragma once
#include <stdbool.h>

/**
 * @brief Структура для хранения параметров программы.
 */
typedef struct parameters_s {
    char* path;          ///< Путь к директории, которую нужно сканировать.
    bool symlinks;       ///< Флаг, указывающий, нужно ли включать символические ссылки в результат.
    bool catalogs;       ///< Флаг, указывающий, нужно ли включать каталоги в результат.
    bool files;          ///< Флаг, указывающий, нужно ли включать обычные файлы в результат.
    bool sort;           ///< Флаг, указывающий, нужно ли сортировать результат по алфавиту.
} parameters_t;
