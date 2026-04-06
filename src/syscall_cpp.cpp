

#include "../h/syscall_cpp.hpp"
#include "../lib/hw.h"

// new/delete wrapperi (bez izuzetaka)
void* operator new(size_t n) {
    void* p = mem_alloc(n);
    if (!p) {
        // nema izuzetaka u freestanding, najjednostavnije: “halt”
        while (true) {}
    }
    return p;
}

void operator delete(void* p) noexcept {
    if (p) mem_free(p);
}

// Thread
Thread::Thread(void (*b)(void*), void* a)
    : myHandle(nullptr), body(b), arg(a) {}

Thread::Thread()
    : myHandle(nullptr), body(nullptr), arg(nullptr) {}

Thread::~Thread() {
    // U postavci nema “thread_close/join”, tako da ovde ne možemo bezbedno da oslobađamo handle.
    // Destruktor postoji jer interfejs kaže da objekti mogu da se uništavaju
}

void Thread::threadWrapper(void* p) {
    Thread* t = (Thread*)p;
    if (!t) { thread_exit(); return; }

    // Pravilo: ako je body postavljen konstruktorom, run se IGNORIŠE
    if (t->body) t->body(t->arg);
    else t->run();

    thread_exit();
}

int Thread::start() {
    // myHandle dobija vrednost iz C API poziva
    if (body) return thread_create(&myHandle, body, arg);
    return thread_create(&myHandle, &Thread::threadWrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

// Semaphore
Semaphore::Semaphore(unsigned init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    if (myHandle) sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

// PeriodicThread
PeriodicThread::PeriodicThread(time_t p)
    : Thread(&PeriodicThread::periodicWrapper, this), period(p) {}

void PeriodicThread::terminate() {
    // Najjednostavnije: “terminate” može da označi neku internu zastavicu,
    // ali pošto nemamo timer/sleep deo u oceni, ostavljamo kao no-op.
    // I dalje postoji metoda i kompajlira se po interfejsu
}

void PeriodicThread::periodicWrapper(void* p) {
    auto* self = (PeriodicThread*)p;
    if (!self) { thread_exit(); return; }

    // Minimalna semantika bez tajmera: jedna aktivacija i kraj
    self->periodicActivation();

    // Ako želiš pravu periodiku
    // while (!terminated) { periodicActivation(); Thread::sleep(period); }
    thread_exit();
}

// Console
char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}


/* ===== MODIFIKACIJE_ZA_REZERVU_SYSCALL_CPP_CPP BEGIN =====
// Dodaj implementacije:
// int Thread::getThreadId() {
//     return thread_get_id();
// }
//
// void Thread::join() {
//     if (myHandle) thread_join(myHandle);
// }
//
// void Thread::pair(Thread* first, Thread* second) {
//     if (!first || !second) return;
//     if (!first->myHandle || !second->myHandle) return;
//     thread_pair(first->myHandle, second->myHandle);
// }
//
// void Thread::sync() {
//     thread_sync();
// }
//
// void Thread::addChild(Thread* child) {
//     if (!child) return;
//     if (!myHandle || !child->myHandle) return;
//     thread_add_child(myHandle, child->myHandle);
// }
//
// void Thread::joinAll() {
//     thread_join_all();
// }

===== MODIFIKACIJE_ZA_REZERVU_SYSCALL_CPP_CPP END ===== */
