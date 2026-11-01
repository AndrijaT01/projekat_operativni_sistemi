

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_H
#define PROJECT_BASE_V1_1_SYSCALL_CPP_H

// #ifndef _syscall_cpp
// #define _syscall_cpp

#include "syscall_c.h"

// Globalni new/delete moraju da obmotavaju mem_alloc/mem_free
void* operator new (size_t );
void  operator delete (void*) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();

    static void dispatch ();
    static int sleep (time_t);

protected:
    Thread ();
    virtual void run () {}

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;

    static void threadWrapper(void* p);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();

    int wait ();
    int signal ();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();

protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}

private:
    time_t period;

    //  koristimo Thread(body,arg) konstruktor, da run() bude ignorisan kad treba
    static void periodicWrapper(void* p);
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

 #endif //PROJECT_BASE_V1_1_SYSCALL_CPP_H
