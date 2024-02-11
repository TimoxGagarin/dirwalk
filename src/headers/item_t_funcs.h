#pragma once
#include "item_t.h"
#include <sys/stat.h>

item_t create_item_t(char* path, struct stat fileStat);
void print_items(item_t* items, int count);
void free_items(item_t* items, int count);