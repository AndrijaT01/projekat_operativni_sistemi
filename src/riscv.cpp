#include "../h/riscv.h"
#include "../h/syscall_dispatch.h"
#include "../lib/console.h"
#include "../lib/hw.h"


static inline bool is_interrupt(uint64 scause) {
    return ((scause >> 63) & 1ULL) != 0;
}

static inline uint64 scause_code(uint64 scause) {
    return scause & 0xfffULL;
}

static inline bool is_ecall(uint64 scause) {
    if (is_interrupt(scause)) return false;
    uint64 code = scause_code(scause);
    return (code == 8 || code == 9);
}

void Riscv::init_trap() {
    w_stvec((uint64)&supervisorTrap);
}
static void printStr(const char* s) {
    while (*s) __putc(*s++);
}

void Riscv::handle_trap(TrapFrame* tf) {
    uint64 sc = tf->scause;

    if (is_ecall(sc)) {
        uint64 code = tf->x[A0];
        uint64 a1   = tf->x[A1];
        uint64 a2   = tf->x[A2];
        uint64 a3   = tf->x[A3];
        uint64 a4   = tf->x[A4];

        uint64 ret = k_syscall_dispatch(code, a1, a2, a3, a4);
        tf->x[A0] = ret;
        tf->sepc += 4;
        return;
    } else if (sc == SCAUSE_CONSOLE) {
        handleConsoleTrap();
        return;
    } else if (sc == SCAUSE_TIMER) {
        handleConsoleTrap();
        return;
    } else if (!is_interrupt(sc) && scause_code(sc) == 2) {
        printStr("Error: Illegal instruction\n");
        while (true) {}
    }


    while (true) {}
}

void Riscv::handleConsoleTrap() {
    uint64 scause = r_scause();

    if (scause == SCAUSE_TIMER) {
        mc_sip(SIP_SSIP);
    } else if (scause == SCAUSE_CONSOLE) {
        mc_sip(SIP_SEIP);
        console_handler();
    } else {
        while (true) {}
    }
}

