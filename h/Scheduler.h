

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H

class TCB;

class Scheduler {
public:
    static void put(TCB* t);
    static TCB* get();

private:
    static TCB* head;
    static TCB* tail;
};

#endif //PROJECT_BASE_V1_1_SCHEDULER_H