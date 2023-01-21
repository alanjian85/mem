#include "mem.h"

#include <stdbool.h>

#include "assert.h"

static void *heap_start, *heap_end;

size_t align(size_t x, size_t n) {
    size_t rem = x % n;
    if (rem != 0)
        x += n - rem;
    return x;
}

void mem_init(void *heap, size_t size) {
    assert((size & 0x1) == 0);

    heap_start = heap;
    heap_end = heap_start + size;

    *((size_t *) heap_start) = size | 0x0;
    *((size_t *) heap_end - 1) = size | 0x0;
}

void *mem_alloc(size_t size) {
    size += sizeof(size_t) << 1;
    size = align(size, 16);
    void *ptr = heap_start;
    size_t block_size = 0;
    bool allocated;
    do {
        ptr += block_size;
        if (ptr >= heap_end)
            return NULL;
        block_size = *((size_t *) ptr) & ~0x1;
        allocated = *((size_t *) ptr) & 0x1;
    } while (block_size < size || allocated);
    block_size -= size;
    if (block_size < sizeof(size_t)) {
        size += block_size;
        block_size = 0;
    } 
    void *end = ptr + size;
    *((size_t *) ptr) = size | 0x1;
    *((size_t *) end - 1) = size | 0x1;
    if (block_size > 0) {
        *((size_t *) end) = block_size | 0x0;
        *((size_t *) (end + block_size) - 1) = block_size | 0x0;
    }
    return ptr + sizeof(size_t);
}

void mem_free(void *ptr) {
    ptr -= sizeof(size_t);
    assert(*((size_t *) ptr) & 0x1);
    *((size_t *) ptr) &= ~0x1;
    size_t size = *((size_t *) ptr);
    void *end = ptr + size;
    *((size_t *) end - 1) &= ~0x1;    

    size_t prev_tag = *((size_t *) ptr - 1);
    if ((prev_tag & 0x1) == 0x0) {
        *((size_t *) (ptr - prev_tag)) += size;   
        *((size_t *) end - 1) += prev_tag; 
    }

    size_t next_tag = *((size_t *) end);
    if ((next_tag & 0x1) == 0x0) {
        *((size_t *) ptr) += next_tag; 
        *((size_t *) (end + next_tag) - 1) += size;
    }
}
