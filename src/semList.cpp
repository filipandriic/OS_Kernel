#include "semList.h"
#include "KernSem.h"

void semList::push_back(KernelSem* kernSem_) {
     LOCKED (
        semNode* node = new semNode(kernSem_);
        if (head == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    )
}

KernelSem* semList::pop() {
    KernelSem* ret = nullptr;
    LOCKED (
        if (head) {
            ret = head->kernSem;
            semNode* old = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
                current = nullptr;
            }
            delete old;
        }
    )
    return ret;
}

void semList::tickAllSems() {
    HARD_LOCKED (
        semNode* temp = head;
        while (temp) {

            temp->kernSem->tickSem();
            temp = temp->next;
        }
    )
    
}

void semList::remove(PCB* pcb_) {
    LOCKED (
        semNode* temp = head;

        while (temp) {
            temp->kernSem->unlimitedWait.remove(pcb_);
            temp->kernSem->limitedWait.remove(pcb_);

            temp = temp->next;
        }
    )
}