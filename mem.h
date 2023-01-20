#ifndef MEM_H
#define MEM_H

#include <stddef.h>

void mem_init(void *heap, size_t size);

void *mem_alloc(size_t size);

void mem_free(void *ptr);

void *mem_calloc(size_t nmemb, size_t size);

void *mem_realloc(void *ptr, size_t size);

#endif // MEM_H
