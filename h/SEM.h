#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H

#pragma once
#include "../lib/hw.h"
#include "../h/TCB.h"

class SEM {
public:
    static SEM* create(unsigned init);
    int close();

    int wait();
    int signal();

private:
    int value;

    TCB* blockedHead = nullptr;
    TCB* blockedTail = nullptr;

    bool closed = false;
    bool destroyRequested = false;

    void block();
    void unblockOne();
    void unblockAll();
    void tryDestroy();
};

#endif