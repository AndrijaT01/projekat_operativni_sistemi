# RISC-V Operating System Kernel

Educational operating-system kernel implemented in **C/C++ and RISC-V assembly**.

## Features

- Thread creation, termination, and cooperative context switching
- FIFO ready-thread scheduling
- Memory allocation and deallocation
- Semaphores and thread synchronization
- System calls through RISC-V traps (`ecall`)
- Console input/output

## Structure

- `src/` — implementation and assembly sources
- `h/` — kernel interfaces and declarations

## Execution model

This kernel uses **cooperative scheduling**, rather than timer-driven preemption. It was implemented for an educational RISC-V environment and is not a production operating system.

## Build

The project contains source files, but the original course runtime, linker setup and toolchain may be required to compile and run it with QEMU.
