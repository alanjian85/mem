#include "mem.h"

#include <stdbool.h>

#include "assert.h"

static size_t *heap_start, *heap_end;

static inline size_t align(size_t x, size_t n) {
    size_t rem = x % n;
    if (rem != 0)
        x += n - rem;
    return x;
}

void mem_init(void *heap, size_t size) {
    assert((size & 0x1) == 0);

    heap_start = (size_t *) align((size_t) heap + 1, 16) - 1;
    heap_end = heap + size;

    *heap_start = size | 0x0;
    *(heap_end - 1) = size | 0x0;
}

void *mem_alloc(size_t size) {
    size += sizeof(size_t) << 1;
    size = align(size, 16);
    size_t *ptr = heap_start;
    size_t block_size = 0;
    bool allocated;
    do {
        ptr = (void *) ptr + block_size;
        if (ptr >= heap_end)
            return NULL;
        block_size = *ptr & ~0x1;
        allocated = *ptr & 0x1;
    } while (block_size < size || allocated);
    block_size -= size;
    if (block_size < sizeof(size_t)) {
        size += block_size;
        block_size = 0;
    } 
    size_t *end = (void *) ptr + size;
    *ptr = size | 0x1;
    *(end - 1) = size | 0x1;
    if (block_size > 0) {
        *end = block_size | 0x0;
        *((size_t *) ((void *) end + block_size) - 1) = block_size | 0x0;
    }
    return ptr + 1;
}

void mem_free(void *ptr) {
    size_t *block = ptr - sizeof(size_t);
    assert(*block & 0x1);
    *block &= ~0x1;
    size_t size = *block;
    size_t *end = (void *) block + size;
    *(end - 1) &= ~0x1;    

    size_t prev_tag = *(block - 1);
    if ((prev_tag & 0x1) == 0x0) {
        *((size_t *) ((void *) block - prev_tag)) += size;   
        *(end - 1) += prev_tag; 
    }

    size_t next_tag = *end;
    if ((next_tag & 0x1) == 0x0) {
        *block += next_tag; 
        *((size_t *) ((void *) end + next_tag) - 1) += size;
    }
}
