#include "mem.h"

#include "assert.h"

static byte *heap_start;
static byte *heap_end;

void mem_init(void *heap, size_t size) {
    assert(size & 0x1 == 0);

    heap_start = (byte *) heap;
    heap_end = heap_start + size;

    *heap_start = size | 0x0;
    *(heap_end - 1) = *heap_start;
}
