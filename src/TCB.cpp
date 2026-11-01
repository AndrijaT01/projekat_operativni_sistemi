#include "../h/TCB.h"
#include "../h/Scheduler.h"
#include "../h/Memory_allocator.h"
#include "../lib/hw.h"
#include "../h/riscv.h"
// za user mode
#include "../h/syscall_c.h"

TCB* TCB::running = nullptr;
TCB* TCB::toFree = nullptr;
TCB* TCB::mainThread = nullptr;
TCB* TCB::idleThread = nullptr;

// placement new bez <new>
void* operator new(size_t, void* p) noexcept { return p; }
void operator delete(void*, void*) noexcept {}

static inline uint64 alignDown16(uint64 x) { return x & ~((uint64)0xF); }

void TCB::idleBody(void*) {
    while (true) {
        TCB::dispatch();
    }
}

void TCB::initIdle() {
    if (idleThread) return;

    void* tcbMem = Memory_allocator::mem_alloc(sizeof(TCB));
    if (!tcbMem) return;

    idleThread = new (tcbMem) TCB(idleBody, nullptr);

    idleThread->stackSize = DEFAULT_STACK_SIZE * sizeof(uint64);
    idleThread->stackBase = Memory_allocator::mem_alloc(idleThread->stackSize);
    if (!idleThread->stackBase) return;

    uint64 stackTop = (uint64)idleThread->stackBase + idleThread->stackSize;
    stackTop = alignDown16(stackTop);

    idleThread->context.sp = stackTop;
    idleThread->context.ra = (uint64)&TCB::threadWrapper;
    idleThread->state = READY;

    // pokusaj za user mode
    idleThread->systemThread = true;

    // NEMOJ Scheduler::put(idleThread);
}

void TCB::initMain() {
    if (mainThread) {
        running = mainThread;
        running->state = RUNNING;
        return;
    }

    void* tcbMem = Memory_allocator::mem_alloc(sizeof(TCB));
    if (!tcbMem) return;

    mainThread = new (tcbMem) TCB(nullptr, nullptr);
    mainThread->state = RUNNING;

    //pokusaj za user mode
    mainThread->systemThread = true;

    // tekući boot/main stek je već aktivan; nemamo zaseban body
    // context ostaje kakav jeste dok prvi dispatch ne sačuva stvarni ra/sp
    running = mainThread;

    initIdle();
}
TCB* TCB::createThread(Body body, void* arg, void* stack_space_end) {
    return createThread(body, arg, stack_space_end, false);
}

TCB* TCB::createThread(Body body, void* arg, void* stack_space_end, bool isSystemThread) {
    if (!body) return nullptr;
    if (!stack_space_end) return nullptr;

    void* tcbMem = Memory_allocator::mem_alloc(sizeof(TCB));
    if (!tcbMem) return nullptr;

    TCB* t = new (tcbMem) TCB(body, arg);

    t->stackSize = DEFAULT_STACK_SIZE * sizeof(uint64);
    t->stackBase = (void*)((uint8*)stack_space_end - t->stackSize);

    uint64 stackTop = (uint64)stack_space_end;
    stackTop = alignDown16(stackTop);

    t->context.sp = stackTop;
    t->context.ra = (uint64)&TCB::threadWrapper;
    t->state = READY;
    t->systemThread = isSystemThread;

    Scheduler::put(t);
    return t;
}

void TCB::dispatch() {
    TCB* old = running;

    if (old && old->state == RUNNING && old != idleThread) {
        old->state = READY;
        Scheduler::put(old);
    }

    TCB* next = Scheduler::get();
    if (!next) next = idleThread;
    if (!next) return;

    next->state = RUNNING;
    running = next;

    if (old == next) return;

    if (old) {
        contextSwitch(&old->context, &next->context);
    } else {
        TCB::Context dummy{};
        contextSwitch(&dummy, &next->context);
    }
}

void TCB::exit() {
    if (!running) return;

    running->state = FINISHED;
    //dispatch();
    yield();

    while (true) {}
}

// pokusaj prebacivanja na user mode
void TCB::threadWrapper() {
    if (running && !running->systemThread) {
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
        Riscv::popSppSpie();
    }

    if (running && running->body) {
        running->body(running->arg);
    }

    if (running && running->systemThread) {
        TCB::exit();
    } else {
        thread_exit();
    }
}

void TCB::tryFreeFinished() {
    // privremeno prazno
}

void TCB::yield() {
    Riscv::pushRegs();
    TCB::dispatch();
    Riscv::popRegs();
}
