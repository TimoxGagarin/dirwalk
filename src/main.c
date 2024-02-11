#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/item_t_funcs.h"

int main(int argc, char *argv[]){
    // Выделение памяти для массива структур item_t.
    // Инициализация с нулевым размером и указанием наличия элементов.
    item_t* items = (item_t*)malloc(0);

    // Инициализация счетчика элементов.
    int count = 0;

    // Получение параметров командной строки.
    parameters_t params = get_options(argc, argv);

    // Сканирование файлов в соответствии с переданными параметрами.
    scan_file(params, &items, &count);

    // Если включена опция сортировки, применение qsort для сортировки элементов.
    if (params.sort)
        qsort(items, count, sizeof(item_t), compare_strings);

    // Вывод элементов на экран.
    print_items(items, count);

    // Освобождение выделенной памяти.
    free_items(items, count);
    
    // Завершение программы с кодом успешного завершения.
    exit(EXIT_SUCCESS);
}