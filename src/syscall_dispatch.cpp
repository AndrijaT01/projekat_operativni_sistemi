

#include "../h/syscall_dispatch.h"
#include "../h/syscall_codes.h"
#include "../h/riscv.h"

#include "../h/Memory_allocator.h"
#include "../h/TCB.h"
#include "../h/SEM.h"
#include "../lib/console.h"
#include "../lib/hw.h"

uint64 k_syscall_dispatch(uint64 code, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {

    switch (code) {
        case SYS_MEM_ALLOC: {
            size_t blocks = (size_t)a1;
            if (blocks == 0) return 0;
            //if (blocks > (SIZE_MAX / MEM_BLOCK_SIZE)) return 0;

            size_t bytes = blocks * MEM_BLOCK_SIZE;
            return (uint64)Memory_allocator::mem_alloc(bytes);
        }

        case SYS_MEM_FREE:
            return (uint64)Memory_allocator::mem_free((void*)a1);

        case SYS_MEM_GET_FREE_SPACE:
            return (uint64)Memory_allocator::mem_get_free_space();

        case SYS_MEM_GET_LARGEST_FREE_BLOCK:
            return (uint64)Memory_allocator::mem_get_largest_free_block();

        case SYS_THREAD_CREATE: {
            // a1=thread_t* handle, a2=start_routine, a3=arg, a4=stack_space_end
            TCB* t = TCB::createThread((TCB::Body)a2, (void*)a3, (void*)a4);
            if (!t) return (uint64)-1;
            if ((void*)a1) *(TCB**)a1 = t;
            return 0;
        }

        case SYS_THREAD_EXIT:
            TCB::exit();
            return 0;

        case SYS_THREAD_DISPATCH:
            TCB::yield();
            return 0;

        case SYS_SEM_OPEN: {
            SEM* s = SEM::create((unsigned)a2);
            if (!s) return (uint64)-1;
            if ((void*)a1) *(SEM**)a1 = s;
            return 0;
        }

        case SYS_SEM_CLOSE:
            return a1 ? (uint64)((SEM*)a1)->close() : (uint64)-1;

        case SYS_SEM_WAIT:
            return a1 ? (uint64)((SEM*)a1)->wait() : (uint64)-1;

        case SYS_SEM_SIGNAL:
            return a1 ? (uint64)((SEM*)a1)->signal() : (uint64)-1;

        case SYS_PUTC:
            __putc((char)a1);
            return 0;

        case SYS_GETC: {
            return (uint64)(uint8)__getc();
            // uint64 oldStvec   = Riscv::r_stvec();
            // uint64 oldSstatus = Riscv::r_sstatus();
            //
            // Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
            // Riscv::w_stvec((uint64)&consoleTrap);
            //
            // char c = __getc();
            //
            // Riscv::w_sstatus(oldSstatus);
            // Riscv::w_stvec(oldStvec);
            //
            // return (uint64)(uint8)c;
        }

        case SYS_TIME_SLEEP:
        default:
            return (uint64)-1;
    }
}
