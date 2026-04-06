
#include "../h/syscall_c.h"
#include "../h/syscall_codes.h"
#include "../lib/hw.h"

static inline uint64 do_ecall(uint64 code, uint64 a1=0, uint64 a2=0, uint64 a3=0, uint64 a4=0) {
    uint64 ret;
    asm volatile(
        "mv a0, %1\n"
        "mv a1, %2\n"
        "mv a2, %3\n"
        "mv a3, %4\n"
        "mv a4, %5\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(ret)
        : "r"(code), "r"(a1), "r"(a2), "r"(a3), "r"(a4)
        : "a0","a1","a2","a3","a4","memory"
    );
    return ret;
}

void* mem_alloc(size_t size) {
    if (size == 0) return nullptr;

    // bytes -> blocks (ceil)
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (void*)do_ecall(SYS_MEM_ALLOC, (uint64)blocks);
}
int mem_free(void* p) {
    return (int)do_ecall(SYS_MEM_FREE, (uint64)p);
}
size_t mem_get_free_space() {
    return (size_t)do_ecall(SYS_MEM_GET_FREE_SPACE);
}

size_t mem_get_largest_free_block() {
    return (size_t)do_ecall(SYS_MEM_GET_LARGEST_FREE_BLOCK);
}

// Threads
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    if (!handle) return -1;

    size_t stackBytes =  DEFAULT_STACK_SIZE * sizeof(uint64);

    void* stack = mem_alloc(stackBytes);
    if (!stack) return -2;

    uint8* stack_end = (uint8*)stack + stackBytes;

    // ABI thread_create ima 4. argument stack_space
    int rc = (int)do_ecall(SYS_THREAD_CREATE,
                           (uint64)handle,
                           (uint64)start_routine,
                           (uint64)arg,
                           (uint64)stack_end);

    // ako nije uspelo, oslobodi stek
    if (rc < 0) mem_free(stack);
    return rc;
}
int thread_exit() {
    return (int)do_ecall(SYS_THREAD_EXIT);
}
void thread_dispatch() {
    (void)do_ecall(SYS_THREAD_DISPATCH);
}

// Semaphores
int sem_open(sem_t* handle, unsigned init) {
    if (!handle) return -1;
    return (int)do_ecall(SYS_SEM_OPEN, (uint64)handle, (uint64)init);
}
int sem_close(sem_t handle) {
    return (int)do_ecall(SYS_SEM_CLOSE, (uint64)handle);
}
int sem_wait(sem_t id) {
    return (int)do_ecall(SYS_SEM_WAIT, (uint64)id);
}
int sem_signal(sem_t id) {
    return (int)do_ecall(SYS_SEM_SIGNAL, (uint64)id);
}

// Sleep (ako ne radiš)
int time_sleep(time_t) { return -1; }

// Console
char getc() { return (char)do_ecall(SYS_GETC); }
void putc(char c) { (void)do_ecall(SYS_PUTC, (uint64)(unsigned char)c); }



/* ===== MODIFIKACIJE_ZA_REZERVU_SYSCALL_C_CPP BEGIN =====
// Dodaj implementacije:
// int thread_get_id() {
//     return (int)do_ecall(SYS_THREAD_GET_ID);
// }
//
// void thread_join(thread_t handle) {
//     (void)do_ecall(SYS_THREAD_JOIN, (uint64)handle);
// }
//
// void thread_pair(thread_t first, thread_t second) {
//     (void)do_ecall(SYS_THREAD_PAIR, (uint64)first, (uint64)second);
// }
//
// void thread_sync() {
//     (void)do_ecall(SYS_THREAD_SYNC);
// }
//
// void thread_add_child(thread_t parent, thread_t child) {
//     (void)do_ecall(SYS_THREAD_ADD_CHILD, (uint64)parent, (uint64)child);
// }
//
// void thread_join_all() {
//     (void)do_ecall(SYS_THREAD_JOIN_ALL);
// }

===== MODIFIKACIJE_ZA_REZERVU_SYSCALL_C_CPP END ===== */
