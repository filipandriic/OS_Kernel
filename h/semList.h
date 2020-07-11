#ifndef _SEMLIST_H_
#define _SEMLIST_H_
#include "locks.h"

class KernelSem;
struct semNode {
    KernelSem* kernSem;
    semNode* next;

    semNode(KernelSem* kernSem_, semNode* next_ = nullptr) : kernSem(kernSem_), next(next_) {}
};

class semList {
private:
    semNode* head;
    semNode* tail;
    semNode* current;

public:
    semList() : head(nullptr), tail(nullptr), current(nullptr) {}

    ~semList() {
        while (head) {
            pop();
        }
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    void push_back(KernelSem* kernSem_);
    KernelSem* pop();
    bool empty() { return !head; }
    void tickAllSems();

    void remove(PCB* pcb_);
};

#endif