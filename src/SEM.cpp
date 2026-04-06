#include "../h/SEM.h"
#include "../h/Memory_allocator.h"
#include "../h/Scheduler.h"
#include "../h/riscv.h"

static inline void bzero(void* p, size_t n) {
    unsigned char* c = (unsigned char*)p;
    for (size_t i = 0; i < n; i++) c[i] = 0;
}

SEM* SEM::create(unsigned init) {
    void* mem = Memory_allocator::mem_alloc(sizeof(SEM));
    if (!mem) return nullptr;

    bzero(mem, sizeof(SEM));
    SEM* s = (SEM*)mem;
    s->value = (int)init;
    s->blockedHead = s->blockedTail = nullptr;
    s->closed = false;
    s->destroyRequested = false;
    //s->refcnt = 0;
    return s;
}

int SEM::close() {
    if (closed) return -1;

    closed = true;
    destroyRequested = true;
    unblockAll();
    tryDestroy();
    return 0;
}

void SEM::block() {
    TCB* t = TCB::running;
    t->state = TCB::BLOCKED;
    t->next = nullptr;

    if (!blockedTail) blockedHead = blockedTail = t;
    else {
        blockedTail->next = t;
        blockedTail = t;
    }

    Riscv::pushRegs();
    TCB::dispatch();
    Riscv::popRegs();
}

void SEM::unblockOne() {
    if (!blockedHead) return;

    TCB* t = blockedHead;
    blockedHead = blockedHead->next;
    if (!blockedHead) blockedTail = nullptr;

    t->next = nullptr;
    t->state = TCB::READY;
    Scheduler::put(t);
}

int SEM::wait() {
    if (closed) return -1;

    value--;
    if (value < 0) {
        block();
        if (closed) return -1;
    }

    return 0;
}

int SEM::signal() {
    if (closed) return -1;

    value++;
    if (value <= 0) {
        unblockOne();
    }

    return 0;
}

void SEM::unblockAll() {
    while (blockedHead) {
        unblockOne();
    }
}

void SEM::tryDestroy() {
    if (destroyRequested && blockedHead == nullptr) {
        Memory_allocator::mem_free(this);
    }
}