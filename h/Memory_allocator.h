

#ifndef PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_H

#include "../lib/hw.h"

struct FreeBlock {
    size_t blocks;  // ukupan broj blokova (header+payload)
    FreeBlock *next;
};

class Memory_allocator {
public:
    static void init();
    static void* mem_alloc(size_t size);
    static int mem_free(void* ptr);
    static size_t mem_get_free_space();
    static size_t mem_get_largest_free_block();
private:
    static bool initialized;
    static FreeBlock* free_mem_head;

};

#endif //PROJECT_BASE_V1_1_MEMORY_ALLOCATOR_H
