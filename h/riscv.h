#ifndef PROJECT_BASE_V1_1_RISCV_H
#define PROJECT_BASE_V1_1_RISCV_H
#pragma once
#include "../lib/hw.h"

struct TrapFrame {
    uint64 x[32];
    uint64 sepc;
    uint64 sstatus;
    uint64 scause;
    uint64 stval;
};

enum RegIndex {
    X0=0, RA=1, SP=2, GP=3, TP=4,
    T0=5, T1=6, T2=7,
    S0=8, S1=9,
    A0=10, A1=11, A2=12, A3=13, A4=14, A5=15, A6=16, A7=17,
    S2=18, S3=19, S4=20, S5=21, S6=22, S7=23, S8=24, S9=25, S10=26, S11=27,
    T3=28, T4=29, T5=30, T6=31
};

class Riscv {
public:
    static inline uint64 r_sstatus() {
        uint64 x;
        asm volatile("csrr %0, sstatus" : "=r"(x));
        return x;
    }

    static inline void w_sstatus(uint64 x) {
        asm volatile("csrw sstatus, %0" : : "r"(x));
    }

    static inline void ms_sstatus(uint64 mask) {
        asm volatile("csrs sstatus, %0" : : "r"(mask));
    }

    static inline void mc_sstatus(uint64 mask) {
        asm volatile("csrc sstatus, %0" : : "r"(mask));
    }

    static inline uint64 r_sie() {
        uint64 x;
        asm volatile("csrr %0, sie" : "=r"(x));
        return x;
    }

    static inline void w_sie(uint64 x) {
        asm volatile("csrw sie, %0" : : "r"(x));
    }

    static inline void ms_sie(uint64 mask) {
        asm volatile("csrs sie, %0" : : "r"(mask));
    }

    static inline uint64 r_stvec() {
        uint64 x;
        asm volatile("csrr %0, stvec" : "=r"(x));
        return x;
    }

    static inline void w_stvec(uint64 x) {
        asm volatile("csrw stvec, %0" : : "r"(x));
    }

    static inline uint64 r_scause() {
        uint64 x;
        asm volatile("csrr %0, scause" : "=r"(x));
        return x;
    }

    static inline void mc_sip(uint64 mask) {
        asm volatile("csrc sip, %0" : : "r"(mask));
    }

    static constexpr uint64 SSTATUS_SIE = (1UL << 1);
    static constexpr uint64 SSTATUS_SPP = (1UL << 8);


    static constexpr uint64 SIE_SEIE    = (1UL << 9);

    static constexpr uint64 SIP_SSIP = (1UL << 1);
    static constexpr uint64 SIP_SEIP = (1UL << 9);

    static constexpr uint64 SCAUSE_TIMER   = (1ULL << 63) | 1;
    static constexpr uint64 SCAUSE_CONSOLE = (1ULL << 63) | 9;

    static constexpr uint64 SIE_SSIE = (1UL << 1); ////




    static void init_trap();

    static void handle_trap(TrapFrame* tf);
    static void handleConsoleTrap();
    static void pushRegs();
    static void popRegs();
    static void popSppSpie();
};

extern "C" void supervisorTrap();
extern "C" void consoleTrap();

#endif