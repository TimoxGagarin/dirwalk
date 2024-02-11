#pragma once
#include "parameters_t.h"
#include "item_t.h"

parameters_t get_options(int argc, char* argv[]);
item_t* scan_file(parameters_t params, item_t** items, int* count);
int compare_strings(const void *a, const void *b);