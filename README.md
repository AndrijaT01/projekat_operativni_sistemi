# RISC-V Operating System Kernel

Educational operating-system kernel written in **C/C++ and RISC-V assembly**, developed as part of an operating-systems course.

## Implemented functionality

- Thread creation, termination and cooperative context switching
- FIFO ready-thread scheduling
- Kernel memory allocation and deallocation
- Semaphores and blocking synchronization
- System-call interface via RISC-V traps (`ecall`)
- Console input/output
- Additional synchronization exercises including join/joinAll and barriers (see source and modification notes)

## Architecture

- `src/` — kernel implementation and assembly sources
- `h/` — header files and interfaces
- `MODIFIKACIJE_README.txt` — notes on course modifications

## Execution model

The kernel uses **cooperative scheduling**: thread switches occur via explicit dispatch or blocking operations, rather than preemptive timer interrupts. It targets a course RISC-V/QEMU environment and is **not** a general-purpose production operating system.

## Running the project

The repository contains the kernel source only. Build/run steps depend on the course's RISC-V compiler, linker scripts, runtime and QEMU setup; these external components are not bundled here.

## Academic integrity

Educational source code: please respect applicable course rules before reusing or submitting any part of this project.
