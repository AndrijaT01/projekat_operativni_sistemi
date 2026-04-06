#ifndef PROJECT_BASE_V1_1_KERNELCONSOLE_H
#define PROJECT_BASE_V1_1_KERNELCONSOLE_H

#include "../lib/hw.h"

class SEM;

class KernelConsole {
public:
    static void init();
    static char getc();
    static void putFromInterrupt(char c);

private:
    static constexpr int CAP = 256;

    static char buffer[CAP];
    static int head;
    static int tail;
    static SEM* items;
    static SEM* spaces;
    static bool initialized;
};

#endif