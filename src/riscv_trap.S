# 1 "src/riscv_trap.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/riscv_trap.S"
    .globl supervisorTrap
    .globl consoleTrap
    .type supervisorTrap, @function
    .type consoleTrap, @function
    .align 2

    .equ TF_SIZE, 304
    .equ OFF_X, 0
    .equ OFF_SEPC, 256
    .equ OFF_SSTATUS, 264
    .equ OFF_SCAUSE, 272
    .equ OFF_STVAL, 280

consoleTrap:
    addi sp, sp, -TF_SIZE

    sd x0, OFF_X + 0*8(sp)
    sd ra, OFF_X + 1*8(sp)
    sd sp, OFF_X + 2*8(sp)
    sd gp, OFF_X + 3*8(sp)
    sd tp, OFF_X + 4*8(sp)
    sd t0, OFF_X + 5*8(sp)
    sd t1, OFF_X + 6*8(sp)
    sd t2, OFF_X + 7*8(sp)
    sd s0, OFF_X + 8*8(sp)
    sd s1, OFF_X + 9*8(sp)
    sd a0, OFF_X + 10*8(sp)
    sd a1, OFF_X + 11*8(sp)
    sd a2, OFF_X + 12*8(sp)
    sd a3, OFF_X + 13*8(sp)
    sd a4, OFF_X + 14*8(sp)
    sd a5, OFF_X + 15*8(sp)
    sd a6, OFF_X + 16*8(sp)
    sd a7, OFF_X + 17*8(sp)
    sd s2, OFF_X + 18*8(sp)
    sd s3, OFF_X + 19*8(sp)
    sd s4, OFF_X + 20*8(sp)
    sd s5, OFF_X + 21*8(sp)
    sd s6, OFF_X + 22*8(sp)
    sd s7, OFF_X + 23*8(sp)
    sd s8, OFF_X + 24*8(sp)
    sd s9, OFF_X + 25*8(sp)
    sd s10, OFF_X + 26*8(sp)
    sd s11, OFF_X + 27*8(sp)
    sd t3, OFF_X + 28*8(sp)
    sd t4, OFF_X + 29*8(sp)
    sd t5, OFF_X + 30*8(sp)
    sd t6, OFF_X + 31*8(sp)

    csrr t0, sepc
    sd t0, OFF_SEPC(sp)

    csrr t0, sstatus
    sd t0, OFF_SSTATUS(sp)

    csrr t0, scause
    sd t0, OFF_SCAUSE(sp)

    csrr t0, stval
    sd t0, OFF_STVAL(sp)

    call _ZN5Riscv17handleConsoleTrapEv

    ld t0, OFF_SEPC(sp)
    csrw sepc, t0

    ld t0, OFF_SSTATUS(sp)
    csrw sstatus, t0

    ld ra, OFF_X + 1*8(sp)
    ld gp, OFF_X + 3*8(sp)
    ld tp, OFF_X + 4*8(sp)
    ld t0, OFF_X + 5*8(sp)
    ld t1, OFF_X + 6*8(sp)
    ld t2, OFF_X + 7*8(sp)
    ld s0, OFF_X + 8*8(sp)
    ld s1, OFF_X + 9*8(sp)
    ld a0, OFF_X + 10*8(sp)
    ld a1, OFF_X + 11*8(sp)
    ld a2, OFF_X + 12*8(sp)
    ld a3, OFF_X + 13*8(sp)
    ld a4, OFF_X + 14*8(sp)
    ld a5, OFF_X + 15*8(sp)
    ld a6, OFF_X + 16*8(sp)
    ld a7, OFF_X + 17*8(sp)
    ld s2, OFF_X + 18*8(sp)
    ld s3, OFF_X + 19*8(sp)
    ld s4, OFF_X + 20*8(sp)
    ld s5, OFF_X + 21*8(sp)
    ld s6, OFF_X + 22*8(sp)
    ld s7, OFF_X + 23*8(sp)
    ld s8, OFF_X + 24*8(sp)
    ld s9, OFF_X + 25*8(sp)
    ld s10, OFF_X + 26*8(sp)
    ld s11, OFF_X + 27*8(sp)
    ld t3, OFF_X + 28*8(sp)
    ld t4, OFF_X + 29*8(sp)
    ld t5, OFF_X + 30*8(sp)
    ld t6, OFF_X + 31*8(sp)

    addi sp, sp, TF_SIZE
    sret

supervisorTrap:
    addi sp, sp, -TF_SIZE

    sd x0, OFF_X + 0*8(sp)
    sd ra, OFF_X + 1*8(sp)
    sd sp, OFF_X + 2*8(sp)
    sd gp, OFF_X + 3*8(sp)
    sd tp, OFF_X + 4*8(sp)
    sd t0, OFF_X + 5*8(sp)
    sd t1, OFF_X + 6*8(sp)
    sd t2, OFF_X + 7*8(sp)
    sd s0, OFF_X + 8*8(sp)
    sd s1, OFF_X + 9*8(sp)
    sd a0, OFF_X + 10*8(sp)
    sd a1, OFF_X + 11*8(sp)
    sd a2, OFF_X + 12*8(sp)
    sd a3, OFF_X + 13*8(sp)
    sd a4, OFF_X + 14*8(sp)
    sd a5, OFF_X + 15*8(sp)
    sd a6, OFF_X + 16*8(sp)
    sd a7, OFF_X + 17*8(sp)
    sd s2, OFF_X + 18*8(sp)
    sd s3, OFF_X + 19*8(sp)
    sd s4, OFF_X + 20*8(sp)
    sd s5, OFF_X + 21*8(sp)
    sd s6, OFF_X + 22*8(sp)
    sd s7, OFF_X + 23*8(sp)
    sd s8, OFF_X + 24*8(sp)
    sd s9, OFF_X + 25*8(sp)
    sd s10, OFF_X + 26*8(sp)
    sd s11, OFF_X + 27*8(sp)
    sd t3, OFF_X + 28*8(sp)
    sd t4, OFF_X + 29*8(sp)
    sd t5, OFF_X + 30*8(sp)
    sd t6, OFF_X + 31*8(sp)

    csrr t0, sepc
    sd t0, OFF_SEPC(sp)

    csrr t0, sstatus
    sd t0, OFF_SSTATUS(sp)

    csrr t0, scause
    sd t0, OFF_SCAUSE(sp)

    csrr t0, stval
    sd t0, OFF_STVAL(sp)

    mv a0, sp
    call _ZN5Riscv11handle_trapEP9TrapFrame

    ld t0, OFF_SEPC(sp)
    csrw sepc, t0

    ld t0, OFF_SSTATUS(sp)
    csrw sstatus, t0

    ld ra, OFF_X + 1*8(sp)
    ld gp, OFF_X + 3*8(sp)
    ld tp, OFF_X + 4*8(sp)
    ld t0, OFF_X + 5*8(sp)
    ld t1, OFF_X + 6*8(sp)
    ld t2, OFF_X + 7*8(sp)
    ld s0, OFF_X + 8*8(sp)
    ld s1, OFF_X + 9*8(sp)
    ld a0, OFF_X + 10*8(sp)
    ld a1, OFF_X + 11*8(sp)
    ld a2, OFF_X + 12*8(sp)
    ld a3, OFF_X + 13*8(sp)
    ld a4, OFF_X + 14*8(sp)
    ld a5, OFF_X + 15*8(sp)
    ld a6, OFF_X + 16*8(sp)
    ld a7, OFF_X + 17*8(sp)
    ld s2, OFF_X + 18*8(sp)
    ld s3, OFF_X + 19*8(sp)
    ld s4, OFF_X + 20*8(sp)
    ld s5, OFF_X + 21*8(sp)
    ld s6, OFF_X + 22*8(sp)
    ld s7, OFF_X + 23*8(sp)
    ld s8, OFF_X + 24*8(sp)
    ld s9, OFF_X + 25*8(sp)
    ld s10, OFF_X + 26*8(sp)
    ld s11, OFF_X + 27*8(sp)
    ld t3, OFF_X + 28*8(sp)
    ld t4, OFF_X + 29*8(sp)
    ld t5, OFF_X + 30*8(sp)
    ld t6, OFF_X + 31*8(sp)

    addi sp, sp, TF_SIZE
    sret
