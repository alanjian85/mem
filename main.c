#include <stdio.h>

#include "mem.h"

#define HEAP_SIZE 4096

static byte heap[HEAP_SIZE];

int main() {
    mem_init(heap, HEAP_SIZE);
    size_t *size = mem_alloc(sizeof(size_t));
    *size = HEAP_SIZE;
    printf("Heap size: %llu\n", size);
    mem_free(size);
    return 0;
}
