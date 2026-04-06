// #include "../h/riscv.h"
// #include "../h/syscall_c.h"
// #include "../h/TCB.h"
// #include "../h/Memory_allocator.h"
// #include "../h/KernelConsole.h"
//
// extern void userMain();
//
// static inline void shutdown_qemu() {
//     volatile unsigned int* p = (unsigned int*)0x100000;
//     *p = 0x5555;
// }
//
// int main() {
//     Memory_allocator::init();
//     Riscv::init_trap();
//     // Omogući external i timer interrupt
//     Riscv::ms_sie(Riscv::SIE_SEIE);
//     Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
//     //KernelConsole::init();
//     TCB::initMain();
//     userMain();
//     shutdown_qemu();
//     return 0;
// }


// pokusaj za user mode
#include "../h/riscv.h"
#include "../h/syscall_c.h"
#include "../h/TCB.h"
#include "../h/Memory_allocator.h"

extern void userMain();

static inline void shutdown_qemu() {
    volatile unsigned int* p = (unsigned int*)0x100000;
    *p = 0x5555;
}

int main() {
    Memory_allocator::init();
    Riscv::init_trap();
    TCB::initMain();

    thread_t userThread = nullptr;
    if (thread_create(&userThread, (void(*)(void*))&userMain, nullptr) < 0) {
        shutdown_qemu();
        return -1;
    }

    while (userThread && !((TCB*)userThread)->isFinished()) {
        thread_dispatch();
    }

    shutdown_qemu();
    return 0;
}