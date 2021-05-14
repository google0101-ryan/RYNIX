#ifndef MEMSET_H
#define MEMSET_H

#include <stddef.h>

void* memset(void* str, int c, size_t bytes);

void* memcpy(void* dest, const void* src, size_t bytes);
#endif