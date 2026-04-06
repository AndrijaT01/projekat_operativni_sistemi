
#include "../h/Memory_allocator.h"


bool Memory_allocator::initialized = false;
FreeBlock* Memory_allocator::free_mem_head = nullptr;

static inline uint8* bptr(void* p) {
    return (uint8*)p;
}
static inline const uint8* bptr(const void* p) {
    return (const uint8*)p;
}

void Memory_allocator::init() {
    if (initialized) return;

    // Heap is [HEAP_START_ADDR, HEAP_END_ADDR)
    const size_t heapBytes = (size_t)(bptr(HEAP_END_ADDR) - bptr(HEAP_START_ADDR));
    const size_t totalBlocks = heapBytes / MEM_BLOCK_SIZE;

    free_mem_head = (FreeBlock*)HEAP_START_ADDR;
    free_mem_head->blocks = totalBlocks;     // includes header block
    free_mem_head->next = nullptr;

    initialized = true;
}

void* Memory_allocator::mem_alloc(size_t size) {
    if (size == 0) return nullptr;

    if (!initialized) init();
    if (!free_mem_head) return nullptr;

    // bytes -> payload blocks (ceil)
    const size_t payloadBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    const size_t needBlocks = payloadBlocks + 1; // +1 header block

    FreeBlock* prev = nullptr;
    FreeBlock* cur = free_mem_head;

    while (cur) {
        if (cur->blocks >= needBlocks) {

            if (cur->blocks == needBlocks) {
                // take whole segment
                if (prev) prev->next = cur->next;
                else free_mem_head = cur->next;
            } else {
                // split
                FreeBlock* rest = (FreeBlock*)(bptr(cur) + needBlocks * MEM_BLOCK_SIZE);

                rest->blocks = cur->blocks - needBlocks;
                rest->next = cur->next;

                if (prev) prev->next = rest;
                else free_mem_head = rest;

                cur->blocks = needBlocks;
            }

            // return payload start (right after header)
            return bptr(cur) + MEM_BLOCK_SIZE;
        }

        prev = cur;
        cur = cur->next;
    }

    return nullptr;
}

int Memory_allocator::mem_free(void* ptr) {
    if (!ptr) return -1;
    if (!initialized) return -2;

    // header is 1 block before payload
    FreeBlock* block = (FreeBlock*)(bptr(ptr) - MEM_BLOCK_SIZE);

    // insert sorted by address
    FreeBlock* prev = nullptr;
    FreeBlock* cur = free_mem_head;

    while (cur && cur < block) {
        prev = cur;
        cur = cur->next;
    }

    block->next = cur;
    if (prev) prev->next = block;
    else free_mem_head = block;

    // coalesce with next
    if (block->next) {
        uint8* blockEnd = bptr(block) + block->blocks * MEM_BLOCK_SIZE;
        if (blockEnd == bptr(block->next)) {
            block->blocks += block->next->blocks;
            block->next = block->next->next;
        }
    }

    // coalesce with prev
    if (prev) {
        uint8* prevEnd = bptr(prev) + prev->blocks * MEM_BLOCK_SIZE;
        if (prevEnd == bptr(block)) {
            prev->blocks += block->blocks;
            prev->next = block->next;
        }
    }

    return 0;
}

size_t Memory_allocator::mem_get_free_space() {
    if (!initialized) init();

    size_t freeBytes = 0;
    for (FreeBlock* cur = free_mem_head; cur; cur = cur->next) {
        // cur->blocks je ukupan broj blokova uključujući header
        if (cur->blocks > 0) freeBytes += cur->blocks * MEM_BLOCK_SIZE;
    }
    return freeBytes;
}

size_t Memory_allocator::mem_get_largest_free_block() {
    if (!initialized) init();

    size_t bestBlocks = 0;
    for (FreeBlock* cur = free_mem_head; cur; cur = cur->next) {
        if (cur->blocks > bestBlocks) bestBlocks = cur->blocks;
    }
    return bestBlocks * MEM_BLOCK_SIZE;
}


