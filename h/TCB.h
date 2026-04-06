#ifndef PROJECT_BASE_V1_1_TCB_H
#define PROJECT_BASE_V1_1_TCB_H

#include "../lib/hw.h"

class Scheduler;

class TCB {
public:
    static void initMain();
    static void initIdle();


    bool isFinished() const { return state == FINISHED; }

    using Body = void(*)(void*);

    struct Context {
        uint64 ra;
        uint64 sp;
        uint64 s0;
        uint64 s1;
        uint64 s2;
        uint64 s3;
        uint64 s4;
        uint64 s5;
        uint64 s6;
        uint64 s7;
        uint64 s8;
        uint64 s9;
        uint64 s10;
        uint64 s11;
    };

    static TCB* createThread(Body body, void* arg, void* stack_space_end);
    static TCB* createThread(Body body, void* arg, void* stack_space_end, bool systemThread);
    static void dispatch();
    static void exit();
    static void yield();

    static TCB* running;
    static TCB* mainThread;
    static TCB* idleThread;


    TCB* next = nullptr;

private:
    enum State { READY, RUNNING, BLOCKED, FINISHED };

    explicit TCB(Body b, void* a) : body(b), arg(a) {}

    static void threadWrapper();
    static void idleBody(void*);

    Context context{};

    void* stackBase = nullptr;
    size_t stackSize = 0;

    Body body = nullptr;
    void* arg = nullptr;

    bool systemThread = false;

    State state = READY;

    static TCB* toFree;
    static void tryFreeFinished();

    friend class Scheduler;
    friend class SEM;
};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);

#endif

/* ===== MODIFIKACIJE_ZA_REZERVU_TCB_H BEGIN =====
// Dodaj blizu include-ova:
// class SEM;

// Dodaj u public deo klase TCB:
// uint64 getId() const { return id; }
// static uint64 getRunningId();
// static void join(TCB* target);
// static void pair(TCB* first, TCB* second);
// static void sync();
// static void addChild(TCB* parent, TCB* child);
// static void joinAll();

// Dodaj u private deo klase TCB:
// void initModResources();
// void notifyParentFinished();
// uint64 id = 0;
// static uint64 nextId;
// SEM* pairSem = nullptr;
// TCB* pairedThread = nullptr;
// bool syncReached = false;
// TCB* parent = nullptr;
// int childCount = 0;
// SEM* childrenSem = nullptr;

===== MODIFIKACIJE_ZA_REZERVU_TCB_H END ===== */
