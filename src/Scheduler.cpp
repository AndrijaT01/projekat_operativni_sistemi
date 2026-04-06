

#include "../h/Scheduler.h"
#include "../h/TCB.h"


TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;

void Scheduler::put(TCB* t) {
    if (!t) return;
    //__putc('P');
    t->next = nullptr;
    if (!tail) head = tail = t;
    else { tail->next = t; tail = t; }
}

TCB* Scheduler::get() {
    if (!head) {
        //__putc('0');
        return nullptr;
    }
    //__putc('G');
    TCB* t = head;
    head = head->next;
    if (!head) tail = nullptr;
    t->next = nullptr;
    return t;
}
