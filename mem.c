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
    *((size_t *) heap_end - sizeof(size_t)) = size;
}

void *mem_alloc(size_t size) {
    size += sizeof(size_t);
    size = align(size, 16);
    void *ptr = heap_start;
    size_t seg_size = 0;
    bool allocated;
    do {
        ptr += seg_size;
        if (ptr >= heap_end)
            return NULL;
        seg_size = *((size_t *) ptr) & ~0x1;
        allocated = *((size_t *) ptr) & 0x1;
    } while (seg_size < size || allocated);
    void *end = ptr + size;
    seg_size -= size;
    if (seg_size < sizeof(size_t)) {
        size += seg_size;
        seg_size = 0;
    } 
    *((size_t *) ptr) = size | 0x1;
    if (seg_size > 0) {
        *((size_t *) end) = seg_size | 0x0;
        *((size_t *) end + seg_size - sizeof(size_t)) = seg_size;
    }
    return ptr + 1;
}

void mem_free(void *ptr) {
    --ptr;
    assert(*((size_t *) ptr) & 0x1);
    *((size_t *) ptr) &= ~0x1;
}
