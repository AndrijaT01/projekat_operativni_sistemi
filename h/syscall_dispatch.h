
#ifndef PROJECT_BASE_V1_1_SYSCALL_DISPATCH_H
#define PROJECT_BASE_V1_1_SYSCALL_DISPATCH_H
#pragma once
#include "../lib/hw.h"

struct TrapFrame;

uint64 k_syscall_dispatch(uint64 code, uint64 a1, uint64 a2, uint64 a3, uint64 a4);
#endif //PROJECT_BASE_V1_1_SYSCALL_DISPATCH_H