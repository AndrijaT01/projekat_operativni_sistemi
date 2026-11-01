

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H

#include "../lib/hw.h"

//MEM
// 0x01
void* mem_alloc(size_t size);

// 0x02
int mem_free(void*);

// 0x03
size_t mem_get_free_space();

// 0x04
size_t mem_get_largest_free_block();

// --- Threads  ---
class _thread;
typedef _thread* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

// --- Semaphores  ---
class _sem;
typedef _sem* sem_t;

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

// --- Sleep  ---
typedef unsigned long time_t;
int time_sleep(time_t);

// --- Console  ---
const int EOF = -1;
char getc();
void putc(char);

#endif //PROJECT_BASE_V1_1_SYSCALL_C_H
