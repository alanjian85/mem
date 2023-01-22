#include <stdio.h>
#include <stdalign.h>

#include "mem.h"

#define HEAP_SIZE 4096

typedef struct {
    int i;
    double j;
    unsigned k;
    void* ptr;
} data_t;

static size_t heap[HEAP_SIZE];

int main() {
    mem_init(heap, HEAP_SIZE);
    void *a = mem_alloc(sizeof(size_t));
    printf("Address: 0x%x\n", a);
    mem_free(a);
    a = mem_alloc(sizeof(int));
    void *b = mem_alloc(sizeof(data_t));
    void *c = mem_alloc(sizeof(size_t));
    printf("Address: 0x%x\n", a);
    printf("Address: 0x%x\n", b);
    printf("Address: 0x%x\n", c);
    mem_free(a);
    mem_free(b);
    a = mem_alloc(sizeof(int));
    b = mem_alloc(sizeof(data_t));
    printf("Address: 0x%x\n", a);
    printf("Address: 0x%x\n", b);
    return 0;
}
